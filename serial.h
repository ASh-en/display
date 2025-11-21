#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QString>
#include <QByteArray>
#include "param_define.h"

/*	start	name:ash	time:20250703	note:
			
*/
#define LEN_TH (9)
#define LEN_EQT (7)
#define LEN_GET (69)
	
			
/*	end		*/
#define __TEST_LOSS_RATE__




const QString THICKNESS_HEAD = "#TH";
const QString WAVE_HEAD = "#ST";
const QString SET_PARAM_HEAD = "#SETOK$"; 
const QString PARAM_CMD_HEAD ="#GET";
const QString STOP_THICK_HEAD = "#STOOK$";
const QString CELIBRATE_THICKNESS_HEAD = "#ONEKEYBD";
const QString POWER_HEAD = "#EQT";

const char HEAD_READ_WAVE[] = {'#', 'S','T'};
const char TAIL_READ_WAVE[] = {0Xaa, 0x55};

static const unsigned char waveTailData[] = {0xaa, 0x55};
static const unsigned char endBagHeadData[] = {0xdc, 0xba};
static const unsigned char beginBagHeadData[] = {0xab, 0xcd};
static const unsigned char beginBagHead30Data[] = {0xab, 0xcd, 0x1d};
static const unsigned char beginBagHead01Data[] = {0xab, 0xcd, 0x00,0x23,0x53,0x54};

const QByteArray WAVE_TAIL = QByteArray::fromRawData(reinterpret_cast<const char*>(waveTailData),2);
const QByteArray END_BAG_TAIL = QByteArray::fromRawData(reinterpret_cast<const char*>(endBagHeadData),2);
const QByteArray BEGIN_BAG_HEAD = QByteArray::fromRawData( reinterpret_cast<const char*>(beginBagHeadData),2);
const QByteArray BEGIN_BAG_HEAD_30 = QByteArray::fromRawData(reinterpret_cast<const char*>(beginBagHead30Data),3);
const QByteArray BEGIN_BAG_HEAD_01 = QByteArray::fromRawData(reinterpret_cast<const char*>(beginBagHead01Data),6);


class serial : public QObject
{
    Q_OBJECT
public:
    explicit serial(QObject *parent = nullptr);


signals:
	void timeSyncReplyReceived(uint16_t timestamp);
    //
	void startTimeSync();//ash 20250715
    void getCharge();
	
    void _charge(const int& data);
    void _thickness(const double& data);
//    void _wave(const QByteArray RecvData);
    void _wave(const WAVE_DATA& cur_data);
    void timesignal();
    void sendthick(double thickness_value);
    void send_para(const QByteArray &para_data);
    void send_dev_params(const DEVICE_ULTRA_PARAM_U &device_params);
public slots:
    void emitsignal();
    void ReadData();
    void tcpRecData(const QByteArray& buf);
    void onReadyRead();
    void timer_elec_quantity_slot();
    void on_timer_get_wave_slot();
	void timeSync();
	void getThk();
    void onParamChanged(INT16 param_no, INT16 param_val);
    void onReadParam();
    void stopThk();
    void StateCheck();

private:
    bool isInTimeSyncProgress = false;
    bool isRecvThicknessData = false;
    bool waitForStopThkResponse = false;
    bool noThkResponse = false;
    void processBuffer();
    void processChargeData();
    void processThicknessData();
    void processWaveData();
    void processParamData(const QByteArray &para_data);
    void processTimeSync();
    void init_device_param(DEVICE_ULTRA_PARAM_U& deviceParam);
    
public:
    QSerialPort serial1;
    QByteArray RecvData;
    QByteArray Recv_charge_Data;
    QByteArray Recv_thick_Data;
    QByteArray Recv_wave_Data;
    QByteArray Recv_loding_Data;
    double thickness_value;
    QTimer *timer_elec_quantity; //
	QTimer *timer3; //同步时钟
	QTimer *timer4;	//获取厚度时钟
    QTimer *timertick;//基准时间步进 5ms
private:
	
	//bool matched = false;
	//bool syncInProgress = false;//ash 20250715
	uint16_t lastSentTimestamp = 0;
    uint16_t timecount = 0;
	#ifdef __TEST_LOSS_RATE__
	uint8_t testcnt = 0; // Packet loss rate test
	uint32_t testloss = 0;
	uint32_t testTotal = 0;
	double testRate = 0;
	#endif
    QEventLoop* syncLoop = nullptr;
    
	
    QTimer *m_timer;
    QThread *m_thread;
    QByteArray m_buffer;
    QByteArray t_buffer;
    QByteArray ts_buffer;
    QByteArray c_buffer;
    QByteArray p_buffer;
    QByteArray w_buffer;
    QByteArray newData;


    DEVICE_ULTRA_PARAM_U mDeviceParam;
    
};

#endif // SERIAL_H
