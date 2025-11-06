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

    QObject::connect(&seri,&serial::send_para,&server,&tcp::send_para_tcp);
    QObject::connect(&seri,&serial::send_para,&w,&Widget::update_para);
    QObject::connect(w.pMeasureForm, &form_measure::sendParamChanged, &seri, &serial::onParamChanged);

    QObject::connect(&seri,&serial::_wave, w.pParamForm,&form_param::onRecvWaveData);
    QObject::connect(&seri, &serial::send_para,w.pParamForm, &form_param::update_para);
    QObject::connect(w.pParamForm, &form_param::sendParamChanged, &seri, &serial::onParamChanged);
    QObject::connect(w.pParamForm, &form_param::GetParam, &seri, &serial::onReadParam);

    if (!seri.serial1.open(QIODevice::ReadWrite)) {
         qDebug() << "Failed to open port" << seri.serial1.portName();
         return 1;
     }

    seri.timeSync();
	seri.timer3->start(1000);
  
    
    //seri.timeSync();
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //seri.serial1.write("#GETTHK%%%%%");

     // 接收数据


     // 发送数据
     QString dataToSend = "Hello, Serial Port!";
     seri.serial1.write(dataToSend.toLocal8Bit());
     seri.timer1->start(10000);
     //seri.timer2->start(3000);
    w.show();

    return a.exec();
}
