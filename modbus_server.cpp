#include "modbus_server.h"
#include <QDebug>

ModbusServer::ModbusServer(QObject *parent) : QObject(parent)
{
    m_server = new QModbusTcpServer(this);
    
    // 连接错误信号
    connect(m_server, &QModbusServer::errorOccurred, this, &ModbusServer::handleError);
    
    // 初始化寄存器
    initializeRegisters();
}

ModbusServer::~ModbusServer()
{
    stopServer();
}

bool ModbusServer::startServer(const QHostAddress &address, quint16 port)
{
    // 如果服务器已经在运行，先停止
    if (m_server->state() == QModbusDevice::ConnectedState) {
        m_server->disconnectDevice();
    }
    
    // 设置服务器参数
    m_server->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    m_server->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address.toString());
    m_server->setServerAddress(1);  // 设置Modbus从机地址为1
    
    // 启动服务器
    if (!m_server->listen()) {
        qDebug() << "Modbus TCP服务器启动失败:" << m_server->errorString();
        return false;
    }
    
    qDebug() << "Modbus TCP服务器已启动，监听地址:" << address.toString() << "端口:" << port;
    return true;
}

void ModbusServer::stopServer()
{
    if (m_server->state() == QModbusDevice::ConnectedState) {
        m_server->disconnectDevice();
        qDebug() << "Modbus TCP服务器已停止";
    }
}

void ModbusServer::setThicknessData(double thickness)
{
    // 将厚度数据（mm）转换为μm并存储到保持寄存器
    quint16 thicknessValue = static_cast<quint16>(thickness * THICKNESS_SCALE_FACTOR);
    
    QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters, THICKNESS_REGISTER_ADDRESS, 1);
    dataUnit.setValue(0, thicknessValue);
    
    if (!m_server->setData(dataUnit)) {
        qDebug() << "设置Modbus寄存器数据失败";
    }
}

bool ModbusServer::isRunning() const
{
    return m_server->state() == QModbusDevice::ConnectedState;
}

void ModbusServer::handleError(QModbusDevice::Error error)
{
    qDebug() << "Modbus服务器错误:" << m_server->errorString();
}

void ModbusServer::initializeRegisters()
{
    // 创建一个数据单元来初始化保持寄存器
    QModbusDataUnit holdingRegs(QModbusDataUnit::HoldingRegisters, 0, 10);  // 初始化10个保持寄存器
    
    // 设置所有寄存器初始值为0
    for (int i = 0; i < holdingRegs.valueCount(); ++i) {
        holdingRegs.setValue(i, 0);
    }
    
    m_server->setData(holdingRegs);
}