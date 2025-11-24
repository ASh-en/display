#ifndef MODBUS_SERVER_H
#define MODBUS_SERVER_H

#include <QObject>
#include <QModbusTcpServer>
#include <QModbusDataUnit>
#include <QHostAddress>
#include "param_define.h"  // 确保包含 PARAM_SIZE 和 DEVICE_ULTRA_PARAM_U 定义
#include <QVector>

class ModbusServer : public QObject
{
    Q_OBJECT

public:
    explicit ModbusServer(QObject *parent = nullptr);
    ~ModbusServer();

    // 启动Modbus TCP服务器
    bool startServer(const QHostAddress &address = QHostAddress::Any, quint16 port = 502);
    
    // 停止服务器
    void stopServer();
    
    // 设置厚度数据（存储在输入寄存器，只读）
    void setThicknessData(bool isConnect, double thickness);
    
    // 获取服务器运行状态
    bool isRunning() const;

public slots:
    // 从外部接收设备参数并更新到保持寄存器
    void updateDeviceParams(const DEVICE_ULTRA_PARAM_U &params);

signals:
    // 保持寄存器值变化时发出（参数：寄存器索引，新值）
    void holdingRegisterChanged(int index, quint16 value);

private slots:
    // 处理Modbus设备错误
    void handleError(QModbusDevice::Error error);
    
    // 关键修改：恢复与 dataWritten 信号匹配的参数列表
    void checkHoldingRegisterChanges(QModbusDataUnit::RegisterType table, int address, int size);

private:
    QModbusTcpServer *m_server;
    
    // 寄存器地址定义（可根据实际需求调整）
    const int THICKNESS_INPUT_REG_ADDRESS = 0;  // 输入寄存器地址（只读）
    const int HOLDING_REGS_START_ADDRESS = 0;   // 保持寄存器起始地址（可读写）
    const int THICKNESS_SCALE_FACTOR = 1000;    // 厚度缩放因子（mm -> μm）
    const int SLAVE_ADDRESS = 1;                // Modbus从机地址
    
    // 初始化寄存器映射和初始值
    void initializeRegisters();
    
    // 缓存保持寄存器当前值（避免重复触发变化信号）
    QVector<quint16> m_holdingRegisterCache;
};

#endif // MODBUS_SERVER_H

