#ifndef WIDGET_H
#define WIDGET_H
#define LBL_DEBUG (0)
#include <QWidget>
#include <QApplication>
#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include "serial.h"
#include <QTimer>
#include "form_calibrate.h"
#include "form_param.h"
#include "form_measure.h"
#include "modbus_server.h"
#include <QDateTime> 

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent=nullptr);
    ~Widget();
    


public slots:

    
    void onBtnMeasureClicked();
    void onBtnCalibrateClicked();
    void onBtnParamClicked();
    void onBtnPoweroffClicked();
    void onBtnRebootClicked();
    void onBtnConnectClicked();
    void onConnectStatus(bool isConnect);
    void onThicknessDataChanged(const double& thickness); // 新增槽函数处理厚度数据变化
    


private slots:
    void onSetStatusTxt(const QString& txt);
    void onHoldingRegisterChanged(int address, quint16 value); // 新增槽函数处理保持寄存器变化
    void onThicknessUpdateTimeout(); // 槽函数处理厚度数据更新超时
    void onRetryRequestTimeout();    // 重试请求槽函数
   

private:
// 测量界面核心组件
    Ui::Widget *ui;
    
    // 辅助函数
    void resetBtnCheckedState();
    void initModbusServer(); // 新增函数初始化Modbus服务器
    QTimer *m_thicknessUpdateTimer; // 用于检测数据更新超时的定时器
    QTimer *m_retryRequestTimer;    // 重发厚度请求定时器
    bool isConnectStatus;           // 用于存储连接状态

    // 用于调试：记录上次更新时间，可选
    qint64 m_lastUpdateTime;
public:
    form_calibrate* pCalibrateForm;
    form_param* pParamForm;
    form_measure* pMeasureForm;
    ModbusServer *m_modbusServer; 

signals:
    void send_status_changed(SEND_STATUS_E status);
    void paramChanged(INT16 param_no, INT16 param_val); // 新增信号通知参数变化
    //请求厚度信号
    void request_thickness();



};

   


#endif // WIDGET_H
