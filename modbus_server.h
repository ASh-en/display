#ifndef MODBUS_SERVER_H
#define MODBUS_SERVER_H

#include <QObject>
#include <QModbusTcpServer>
#include <QModbusDataUnit>
#include <QHostAddress>

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
    
    // 设置厚度数据
    void setThicknessData(double thickness);
    
    // 获取服务器状态
    bool isRunning() const;

private slots:
    // 处理连接错误
    void handleError(QModbusDevice::Error error);

private:
    QModbusTcpServer *m_server;
    const int THICKNESS_REGISTER_ADDRESS = 0;  // 厚度数据寄存器地址
    const int THICKNESS_SCALE_FACTOR = 1;   // 缩放因子，将mm转换为μm以存储为整数
    
    // 初始化Modbus寄存器
    void initializeRegisters();
};

#endif // MODBUS_SERVER_H