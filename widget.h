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
   

private:
// 测量界面核心组件
    Ui::Widget *ui;
    
     bool isConnectStatus=false;
    // 辅助函数
    void resetBtnCheckedState();
    void initModbusServer(); // 新增函数初始化Modbus服务器
public:
    form_calibrate* pCalibrateForm;
    form_param* pParamForm;
    form_measure* pMeasureForm;
    QTimer* p_timer_get_wave;
    ModbusServer *m_modbusServer; 

signals:
    void send_start_thick();
    void paramChanged(INT16 param_no, INT16 param_val); // 新增信号通知参数变化
    void getAllParam_S();



};

   


#endif // WIDGET_H
