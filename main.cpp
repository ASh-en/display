#include <QApplication>
#include "TimeUtils.h"
#include "widget.h"
#include "tcp.h"
#include "serial.h"
#include "dac8760.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tcp server;
    serial seri;
    Widget w;
    QObject::connect(&server,&tcp::tcp_rec,&seri,&serial::tcpRecData);
    QObject::connect(&seri,&serial::_charge,w.pMeasureForm,&form_measure::onBatteryQuantityChanged);
    
    QObject::connect(&seri,&serial::_thickness,w.pMeasureForm,&form_measure::onRecvThhicknesData);
    QObject::connect(&seri,&serial::_thickness,w.pCalibrateForm,&form_calibrate::on_recv_thickness);

    QObject::connect(w.p_timer_get_wave,&QTimer::timeout, &seri, &serial::on_timer_get_wave_slot);
    QObject::connect(&seri,&serial::send_connect_status,&w,&Widget::onConnectStatus);
    QObject::connect(&seri,&serial::send_dev_params,w.m_modbusServer,&ModbusServer::updateDeviceParams);
    QObject::connect(&seri,&serial::send_dev_params,w.pParamForm,&form_param::updateDeviceParams);
    QObject::connect(&seri,&serial::send_dev_params,w.pCalibrateForm,&form_calibrate::updateDeviceParams);
    
    QObject::connect(&seri,&serial::_wave, w.pParamForm,&form_param::updatePlotGraph);

    //QObject::connect(&seri,&serial::send_para,&server,&tcp::send_para_tcp);
    QObject::connect(w.pMeasureForm, &form_measure::sendParamChanged, &seri, &serial::onParamChanged);
    QObject::connect(w.pParamForm, &form_param::sendParamChanged, &seri, &serial::onParamChanged);
    QObject::connect(w.pParamForm, &form_param::GetParam, &seri, &serial::onReadParam);
    QObject::connect(&w, &Widget::send_start_thick, &seri, &serial::getThk);
    QObject::connect(&w, &Widget::getAllParam_S, &seri, &serial::onReadParam);

    
    

    if (!seri.serial1.open(QIODevice::ReadWrite)) {
         qDebug() << "Failed to open port" << seri.serial1.portName();
         return 1;
     }
    seri.onReadParam();

    seri.timeSync();
	//seri.timer3->start(1000);

  
    
    //seri.timeSync();
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //seri.serial1.write("#GETTHK%%%%%");

     // 接收数据


     // 发送数据
     QString dataToSend = "Hello, Serial Port!";
     seri.serial1.write(dataToSend.toLocal8Bit());
     
     //seri.timer2->start(3000);
    w.show();

    return a.exec();
}
