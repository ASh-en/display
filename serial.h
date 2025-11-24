#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QCoreApplication>
// #include <QSerialPort>
// #include <QSerialPortInfo>
// #include <QDebug>
// #include <QThread>
// #include <QTimer>
// #include <QString>
// #include <QByteArray>
// #include "param_define.h"
#include "serial_communicate.h"
#include "param_define.h"
#include "prasingRecvCmd.h"
#include <QTimer>

	
			
// /*	end		*/
// #define __TEST_LOSS_RATE__




class serial : public QObject
{
    Q_OBJECT
public:
    explicit serial(QObject *parent = nullptr);


signals:
	void timeSyncReplyReceived(uint16_t timestamp);
	void startTimeSync();//ash 20250715
    void _delayUpdated(const uint16_t delta);   // 传递当前时延（毫秒级）
    void _charge(const int& data);
    void _thickness(const double& data);

    void _wave(const WAVE_DATA& cur_data);
    void send_para(const QByteArray &para_data);
    void send_dev_params(const DEVICE_ULTRA_PARAM_U &device_params);
    void send_connect_status(bool isMactched);
public slots:
    //void emitsignal();
    void ReadData();
    void tcpRecData(const QByteArray& buf);
    void onReadyRead();
    //void timer_elec_quantity_slot();
    // void on_timer_get_wave_slot();
	//void timeSync();
	void getThk();
    void onParamChanged(INT16 param_no, INT16 param_val);
    void onReadParam();
    void stopThk();
    //void StateCheck();
    void onConnectSeial();
    
    

private:
    
   
    void processThicknessData(int index);
    void processWaveData();
    void processTimeSync();
    void init_device_param(DEVICE_ULTRA_PARAM_U& deviceParam);

    serial_communicate* p_serial_communicate;
    bool isConnectSerial;
    QByteArray recvBuff;	
	quint64 recvThickNum;
    prasingRecvCmd  mRecvCmd;
    DEVICE_ULTRA_PARAM_U mDiviceParam;
    quint64 recvWaveNum;
    int prasing_error_num;
    RECV_PRASING_DATA curRecvData;
    SEND_STATUS_E mSendStatus;

    
    
public:
   void setSendStatus(SEND_STATUS_E status);
private:
    QTimer* pTimerSendStatus;
    DEVICE_ULTRA_PARAM_U mDeviceParam;
private slots:
    void onTimerSendStatusSlots();
    
};

#endif // SERIAL_H
