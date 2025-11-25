#include "modbus_server.h"
#include <QDebug>
#include <QtGlobal>
#include <QtEndian> 

ModbusServer::ModbusServer(QObject *parent) : QObject(parent)
{
     m_server = new QModbusTcpServer(this);

    // 信号槽绑定必须在初始化寄存器之后
    initializeRegisters();

    // 关键修改：连接 dataWritten 信号到带参数的槽函数
    connect(m_server, &QModbusServer::errorOccurred, this, &ModbusServer::handleError);
    connect(m_server, &QModbusServer::dataWritten, this, &ModbusServer::checkHoldingRegisterChanges);
    
    // 校验 PARAM_SIZE 有效性（避免缓存大小异常）
    Q_ASSERT_X(PARAM_SIZE > 0 && PARAM_SIZE <= 1000, "ModbusServer", "PARAM_SIZE 必须是 1~1000 的整数");
    m_holdingRegisterCache.resize(PARAM_SIZE);
    m_holdingRegisterCache.fill(0);
}

ModbusServer::~ModbusServer()
{
    stopServer();
    delete m_server; 
}

// 启动Modbus服务器
bool ModbusServer::startServer(const QHostAddress &address, quint16 port)
{
    if (m_server->state() == QModbusDevice::ConnectedState) {
        m_server->disconnectDevice();
    }

    // 设置网络参数和从机地址
    m_server->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    m_server->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address.toString());
    m_server->setServerAddress(SLAVE_ADDRESS);

    if (!m_server->connectDevice()) {
        qDebug() << "[Modbus] 启动失败:" << m_server->errorString();
        return false;
    }
    qDebug() << "[Modbus] 启动成功，监听" << address.toString() << ":" << port;
    return true;
}

// 停止Modbus服务器
void ModbusServer::stopServer()
{
    if (m_server->state() == QModbusDevice::ConnectedState) {
        m_server->disconnectDevice();
        qDebug() << "[Modbus] 服务器已停止";
    }
}

// 设置厚度数据（写入输入寄存器）
void ModbusServer::setThicknessData(bool isConnect, double thickness)
{
    // 1. 厚度转换：mm -> μm
    quint16 scaled = static_cast<quint16>(thickness * THICKNESS_SCALE_FACTOR);
    quint16 connectFlag = isConnect ? 1 : 0;
    // 2. 关键修改：将数据转换为大端字节序
    quint16 bigEndianValue = qToBigEndian(scaled);
    quint16 littleEndianValue = qToLittleEndian(scaled);

    // 3. 写入转换后的大端值
    if (!m_server->setData(QModbusDataUnit::InputRegisters,
                           THICKNESS_INPUT_REG_ADDRESS,
                           bigEndianValue)) { // 传入大端值
        qDebug() << "[Modbus] 厚度写入失败:" << m_server->errorString();
    } else {
        // qDebug() << "[Modbus] 厚度更新：" << thickness << "mm -> 大端值" << QString("0x%1").arg(bigEndianValue, 4, 16, QChar('0'));
    }
    m_server->setData(QModbusDataUnit::InputRegisters,
                           THICKNESS_INPUT_REG_ADDRESS+1,
                           littleEndianValue);
    m_server->setData(QModbusDataUnit::InputRegisters,
                           THICKNESS_INPUT_REG_ADDRESS+2,
                           connectFlag);
}

// 从外部更新设备参数（写入保持寄存器）
void ModbusServer::updateDeviceParams(const DEVICE_ULTRA_PARAM_U &params)
{
    QModbusDataUnit holdingUnit(QModbusDataUnit::HoldingRegisters,
                                HOLDING_REGS_START_ADDRESS,
                                PARAM_SIZE);

    for (int i = 0; i < PARAM_SIZE; ++i) {
        // 1. 将参数值转换为 quint16
        quint16 value = static_cast<quint16>(params.arrParam[i].value);

        // 2. 关键修改：将每个值转换为大端字节序
        quint16 bigEndianValue = qToLittleEndian(value);

        // 3. 写入转换后的大端值
        holdingUnit.setValue(i, bigEndianValue); // 传入大端值
    }

    if (!m_server->setData(holdingUnit)) {
        qDebug() << "[Modbus] 设备参数写入失败:" << m_server->errorString();
    } else {
        qDebug() << "[Modbus] 设备参数更新成功，共" << PARAM_SIZE << "个参数 (已转换为大端)";
        // 注意：缓存中也应存储大端值，以确保后续比较的一致性
        for (int i = 0; i < PARAM_SIZE; ++i) {
            m_holdingRegisterCache[i] = static_cast<quint16>(params.arrParam[i].value);
            qDebug()<<m_holdingRegisterCache[i];
        }
    }
}

// 关键修改：修复保持寄存器变化检测逻辑
void ModbusServer::checkHoldingRegisterChanges(QModbusDataUnit::RegisterType table, int address, int size)
{
    // 1. 只处理保持寄存器的写入事件（过滤线圈、离散输入等无关类型）
    if (table != QModbusDataUnit::HoldingRegisters) {
        return;
    }

    // 2. 校验写入地址是否在监控范围内（避免处理无关地址）
    const int endAddress = address + size - 1;  // 写入的结束地址（闭区间）
    const int monitorEndAddress = HOLDING_REGS_START_ADDRESS + PARAM_SIZE - 1;
    if (address < HOLDING_REGS_START_ADDRESS || endAddress > monitorEndAddress) {
        qDebug() << "[Modbus] 忽略超出监控范围的保持寄存器写入：地址" << address << "-" << endAddress;
        return;
    }

    // 3. 循环读取单个保持寄存器（避免 QModbusServer::data() 批量读取错误）
    QVector<quint16> currentValues(PARAM_SIZE);
    bool readSuccess = true;
    for (int i = 0; i < PARAM_SIZE; ++i) {
        const int regAddress = HOLDING_REGS_START_ADDRESS + i;  // 当前寄存器地址
        quint16 regValue = 0;

        // 读取单个寄存器的值（data() 函数正确用法：类型 + 地址 + 输出值）
        if (!m_server->data(QModbusDataUnit::HoldingRegisters, regAddress, &regValue)) {
            qDebug() << "[Modbus] 读取保持寄存器" << regAddress << "失败:" << m_server->errorString();
            readSuccess = false;
            break;
        }

        currentValues[i] = regValue;
    }

    // 读取失败则直接返回
    if (!readSuccess) {
        return;
    }
    quint16  currecntValues_s ;
    // 4. 对比缓存，检测寄存器值变化
    for (int i = 0; i < currentValues.size(); ++i) {
        if (currentValues[i] != m_holdingRegisterCache[i]) {
            currecntValues_s = currentValues[i];
            // 发出变化信号（参数：寄存器索引，新值）
            emit holdingRegisterChanged(i, currecntValues_s);
            // 更新缓存（避免下次重复触发）
            m_holdingRegisterCache[i] = currentValues[i];
            qDebug() << "[Modbus] 保持寄存器变化：索引" << i << "（地址" << HOLDING_REGS_START_ADDRESS + i << "），新值" << currentValues[i];
        }
    }
}

// 处理Modbus设备错误
void ModbusServer::handleError(QModbusDevice::Error error)
{
    qDebug() << "[Modbus] 错误：" << m_server->errorString() << "（错误码：" << error << "）";
}

void ModbusServer::initializeRegisters()
{

     QModbusDataUnitMap reg;
        reg.insert(QModbusDataUnit::Coils, { QModbusDataUnit::Coils, 0, 10 });
        reg.insert(QModbusDataUnit::DiscreteInputs, { QModbusDataUnit::DiscreteInputs, 0, 10 });
        reg.insert(QModbusDataUnit::InputRegisters, { QModbusDataUnit::InputRegisters, THICKNESS_INPUT_REG_ADDRESS, 10 });
        reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, HOLDING_REGS_START_ADDRESS, PARAM_SIZE });

        m_server->setMap(reg);
}

// 检查服务器是否运行
bool ModbusServer::isRunning() const
{
    return m_server->state() == QModbusDevice::ConnectedState;
}