#include "serial.h"
#include <QThread>
#include "TimeUtils.h"
#include <QtEndian> // 需要包含这个头文件
#include <QDebug>
#include "g_var.h"



static INT8 sendCommandList[SEND_COMMAND_LENGTH + 1];

serial::serial(QObject *parent) : QObject(parent)
{
	
	p_serial_communicate = new serial_communicate();
    init_device_param(mDeviceParam);
	pTimerSendStatus = new QTimer(this);
	QObject::connect(pTimerSendStatus,&QTimer::timeout, this, &serial::onTimerSendStatusSlots);
	mSendStatus = SEND_THICK_STATUS;
	pTimerSendStatus->start(100);
	
	

}

void serial::tcpRecData(const QByteArray& buf){
	

}

void serial::onConnectSeial()
{
	if(isConnectSerial)
	{
		qDebug()<<"已连接串口";
		return;
	}

	if (!p_serial_communicate->open_serial_port())
	{
		qDebug()<<"串口连接失败";
		return;
	}
	
	connect(&p_serial_communicate->serial_port, &QSerialPort::readyRead, this, &serial::onReadyRead);
	isConnectSerial= true;
	p_serial_communicate->add_send_command_list(SEND_COMMAND_GET_ALL_PARAM);
	p_serial_communicate->add_send_command_list(SEND_COMMAND_STOP_THICK);
	p_serial_communicate->add_send_command_list(SEND_COMMAND_GET_THICK);
	
}
	

void serial::init_device_param(DEVICE_ULTRA_PARAM_U& deviceParam)
{

	// 参数项与现有协议不符合，所以先注释掉
    // deviceParam.stParam.gate2End.index = 0;
    // deviceParam.stParam.gate2End.value = 300;

    // deviceParam.stParam.zeroPointEnvelope.index = 1;
    // deviceParam.stParam.zeroPointEnvelope.value = 2470;

    // deviceParam.stParam.ultraSpeed.index = 2;
    // deviceParam.stParam.ultraSpeed.value = 32500;

    // deviceParam.stParam.zeroPointOrigin.index = 3;
    // deviceParam.stParam.zeroPointOrigin.value = 2470;

    // deviceParam.stParam.gate2Start.index = 4;
    // deviceParam.stParam.gate2Start.value = 200;

    // deviceParam.stParam.gate1Start.index = 5;
    // deviceParam.stParam.gate1Start.value = 100;

    // deviceParam.stParam.gate1End.index = 6;
    // deviceParam.stParam.gate1End.value = 180;

    // deviceParam.stParam.calibrationThick.index = 7;
    // deviceParam.stParam.calibrationThick.value = 1200;

    // deviceParam.stParam.temperature.index = 8;
    // deviceParam.stParam.temperature.value = 25;

    // deviceParam.stParam.curveSmooth.index = 9;
    // deviceParam.stParam.curveSmooth.value = USING_CURVE_SMOOTH;

    // deviceParam.stParam.smoothTime.index = 10;
    // deviceParam.stParam.smoothTime.value = 4;

    // deviceParam.stParam.sensorGain.index = 11;
    // deviceParam.stParam.sensorGain.value = 300;

    // deviceParam.stParam.ferqSelect.index = 12;
    // deviceParam.stParam.ferqSelect.value = LOW_FREQUENCY;

    // deviceParam.stParam.waveType.index = 13;
    // deviceParam.stParam.waveType.value = ORIGIN_WAVE;

    // deviceParam.stParam.measureMode.index = 14;
    // deviceParam.stParam.measureMode.value = MEASURE_AUTO;

    // deviceParam.stParam.SNR.index = 15;
    // deviceParam.stParam.SNR.value = 10;

    // deviceParam.stParam.dampingRadio.index = 16;
    // deviceParam.stParam.dampingRadio.value = 10;

    // deviceParam.stParam.envelopeWidth.index = 17;
    // deviceParam.stParam.envelopeWidth.value = 2470;

    // deviceParam.stParam.widthHeightRadio.index = 18;
    // deviceParam.stParam.widthHeightRadio.value = 10;

    // deviceParam.stParam.thresholdValue.index = 19;
    // deviceParam.stParam.thresholdValue.value = 10;

    // deviceParam.stParam.emitInterval.index = 20;
    // deviceParam.stParam.emitInterval.value = 1000;

    // deviceParam.stParam.pulseNumber.index = 21;
    // deviceParam.stParam.pulseNumber.value = 2;

    // deviceParam.stParam.excitationFrequency.index = 22;
    // deviceParam.stParam.excitationFrequency.value = 500;

    // deviceParam.stParam.startDisplay.index = 23;
    // deviceParam.stParam.startDisplay.value = 2;

    // deviceParam.stParam.stopDisplay.index = 24;
    // deviceParam.stParam.stopDisplay.value = 30;

    // deviceParam.stParam.communicateMode.index = 25;
    // deviceParam.stParam.communicateMode.value = 0;

    // deviceParam.stParam.measureControlMode.index = 26;
    // deviceParam.stParam.measureControlMode.value = PP_CONTROL_MODE;

    // deviceParam.stParam.manualMeasureMode.index = 27;
    // deviceParam.stParam.manualMeasureMode.value = MANUAL_PP_CONTROL_MODE;

    // deviceParam.stParam.lcdDisplayTxt.index = 28;
    // deviceParam.stParam.lcdDisplayTxt.value = DISPLAY_THICKNESS;

    // deviceParam.stParam.selfMultiplying.index = 29;
    // deviceParam.stParam.selfMultiplying.value = SELF_MULTIPLYING;

    // deviceParam.stParam.lcdDisplayTxt.index = 30;
    // deviceParam.stParam.thicknessSmooth.value = 30;

    // deviceParam.stParam.lcdDisplayTxt.index = 31;
    // deviceParam.stParam.thicknessSmooth.value = TITANIUM_ALLOY;
}




// {
//     if (para_data.size() < sizeof(DEVICE_PARAM_S) / sizeof(PARAM_STRUCT_S) * 2) {
//         qDebug() << "参数数据长度不足，无法解析。";
//         return;
//     }

//     const char *dataPtr = para_data.constData();

//     for (int i = 0; i < PARAM_SIZE; i++) {
//         // qFromBigEndian 会从指定地址读取 sizeof(T) 个字节，并转换为本地字节序
//         quint16 value = qFromBigEndian<quint16>(dataPtr + i * 2);
//         mDeviceParam.arrParam[i].index = i;
//         mDeviceParam.arrParam[i].value = value;
//     }
// 	emit send_dev_params(mDeviceParam);

// }











void serial::onParamChanged(INT16 param_no, INT16 param_val)
{
	INT16 sendParam[2] = { 0 };
	sendParam[0] = convert2LittleBigEndian(param_no);
	sendParam[1] = convert2LittleBigEndian(param_val);
    p_serial_communicate->add_send_command_list(SEND_COMMAND_SEND_PARAM, (INT8*)sendParam, sizeof(sendParam));
}
void serial::onReadParam(){
}


void serial::ReadData()
{

}


void serial::onReadyRead()
{
	WAVE_DATA waveData;
	QByteArray recv = p_serial_communicate->serial_port.readAll();
	recvBuff.push_back(recv);
	//qDebug()<< recv.size();

	while (MIN_DATA_LEN <= recvBuff.size())
	{
		memset(&curRecvData, 0, sizeof(curRecvData));
		if (0 > mRecvCmd.parsingCommand(recvBuff, curRecvData))
		{
			prasing_error_num++;
			break;
		}
		
		if (0 == curRecvData.cmdType)
		{
			break;
		}
		//qDebug() << curRecvData.cmdType;
		prasing_error_num = 0;
		continue_send_cmd[curRecvData.cmdType] = false;
		switch (curRecvData.cmdType)
		{
            case RECV_COMMAND_SEND_PARAM:
                //set_status_txt(QString::fromLocal8Bit("设置成功"));
                break;
            case RECV_COMMAND_GET_ALL_PARAM:
            {
                   //set_status_txt(QString::fromLocal8Bit("获取全部参数成功"));
                for (int i = 0; i < PARAM_SIZE; i++)
                {
                    mDiviceParam.arrParam[i].index = i;
                    mDiviceParam.arrParam[i].value = curRecvData.arrResult[i];
                }
                emit send_dev_params(mDiviceParam);
                
                break;
            }
            case RECV_COMMAND_GET_THICK:
            {
                recvThickNum++;
                static int consecutiveTimeouts = 0;   

                uint8_t count = (uint8_t)curRecvData.arrResult[0];
                uint16_t thickness = static_cast<uint16_t>(curRecvData.arrResult[1]);
                uint16_t timestamp = static_cast<uint16_t>(curRecvData.arrResult[2]);
                qDebug()<<"count"<<count<<"thickness:"<<thickness<<"timestamp"<<timestamp;

                uint8_t lossdelta = (uint8_t)(count - testcnt); // 自动处理回绕
                testcnt = count + 1;
                testloss += lossdelta;
                testTotal += (lossdelta==0) ? 1 : lossdelta;
                testRate = 100.0 * testloss / testTotal;
                if(lossdelta != 0)
                {
                    //m_buffer.remove(0, index);  //Todo:丢弃当前数据包之前的数据，可能会导致其他数据（主要为电量数据）被遗弃,需要进一步优化
                    qDebug().noquote() << QString("Delta: %1 Total: %2 Loss Rate: %3%")
                                            .arg(lossdelta)
                                            .arg(testTotal)
                                            .arg(testRate, 0, 'f', 2);
                }

                uint16_t now = static_cast<uint16_t>(TimeUtils::steadyTimeMillis() % 60000);
                uint16_t delta = now > timestamp ? (now - timestamp) : (now + 60000 - timestamp);
            
                qDebug()<<"now: "<< now << "timestamp: " << timestamp <<"delta: " << delta;
                //emit _delayUpdated(delta);

                
                if(delta > 400)
                {
                    qDebug() << "TimeOut detected. delta=" << delta;
                    consecutiveTimeouts++;
                    
                    thickness = 0;

                    if (consecutiveTimeouts >= 10) 
                    {
                        consecutiveTimeouts = 0;        // 触发后立即清零

                        qDebug() << "Consecutive timeouts reached threshold. Restarting time sync.";
                        p_serial_communicate->clear_g_send_cmd_lst();
                        
                        stopThk();                      // 停止获取厚度数据
                        matched = false;
                        
                        timeSync();                     // 重新触发校时
                        
                        
                    }		
                    
                }
                else
                {
                    consecutiveTimeouts = 0;            // 非连续重置超时计数器
                }

                emit _thickness(static_cast<UINT16>(thickness));
                // ui->ldt_thick->setText(QString::number(static_cast<UINT16>(curRecvData.arrResult[0]) / 1000.0, 'f', 3));
                //set_status_txt(QString::fromLocal8Bit("读取厚度成功[") + QString::number(recvThickNum) + "]");
                break;
            }
            case RECV_COMMAND_READ_WAVE:
            {
                recvWaveNum++;
                //set_status_txt(QString::fromLocal8Bit("读取波形成功[" ) + QString::number(recvWaveNum) + "]");
                waveData.thickness = curRecvData.arrResult[0] / 1000.0;
                waveData.pos_first = curRecvData.arrResult[1] / 100.0;
                waveData.pos_second = curRecvData.arrResult[2] / 100.0;
                waveData.gain = curRecvData.arrResult[3] / 10.0;
                waveData.freq = curRecvData.arrResult[4] / 100.0;
                waveData.control_mode = curRecvData.arrResult[5] == 0 ? PP_CONTROL_MODE : AUTO_CONTROL_MODE;
                for (int i = 0; i < WAVE_NUM; i++)
                {
                    waveData.wave_data[i] =  (double)curRecvData.arrResult[i + 6] ;
                }
                emit _wave(waveData);
                break;
            }
            
            case RECV_COMMAND_ELECTRIC_QT:
            {
                qDebug()<<"charge"<<curRecvData.arrResult[0];
                emit _charge(static_cast<UINT16>(curRecvData.arrResult[0]));
                break;
            }
            case RECV_COMMAND_SYNC_TIME:
            {
                uint16_t recv_ts = static_cast<uint16_t>(curRecvData.arrResult[0]);
                // 解析接收到的时间戳;
                qDebug() << "[TimeSync] Received ts:" << recv_ts
                        << "expected:" << lastSentTimestamp;

                matched = (recv_ts == lastSentTimestamp);
                if (matched)
                {

                    qDebug() << "Time synchronized successfully.";
                    // 获取厚度数据
                    getThk();
                    // 重置同步状态
                }
                else
                {
                    // 时间戳不匹配，重新触发校时
                    qDebug() << "Time synchronized failed. Retrying...";
                    timeSync();
                }

                break;
            }
            // default:
            // 	break;
		}
	}
	if (8000 < recvBuff.size())
	{
		recvBuff.clear();
	}
}

void serial::getThk()
{
    p_serial_communicate->add_send_command_list(SEND_COMMAND_GET_THICK);
}

void serial::stopThk()
{
    p_serial_communicate->add_send_command_list(SEND_COMMAND_STOP_THICK);
}

void serial::closeSerial()
{
    if(!isConnectSerial)
    {
        return;
    }
    p_serial_communicate->close_serial_port();
    isConnectSerial = false;
}


void serial::setSendStatus(SEND_STATUS_E status)
{
	mSendStatus = status;
}

void serial::onTimerSendStatusSlots()
{
	static SEND_STATUS_E former_status = SEND_STATUS_NONE;
	static quint64 send_wave_count = 0, send_elec_quantity =0;
	if(former_status != mSendStatus)
	{
        //状态变换清除原有的发送队列
        p_serial_communicate->clear_g_send_cmd_lst();
		if( NOT_SEND_THICK_STAUS == mSendStatus)
		{
			p_serial_communicate->add_send_command_list(SEND_COMMAND_STOP_THICK);
		}
		else
		{
			p_serial_communicate->add_send_command_list(SEND_COMMAND_GET_THICK);
		}

		former_status = mSendStatus;
	}
	send_wave_count +=1;
	send_elec_quantity +=1;
	//非测量状态：每隔3S发送一次波形和参数
	if(0 == send_wave_count % 100 && NOT_SEND_THICK_STAUS == mSendStatus)
	{
		p_serial_communicate->add_send_command_list(SEND_COMMAND_READ_WAVE);
		p_serial_communicate->add_send_command_list(SEND_COMMAND_GET_ALL_PARAM);
	}
	//测量状态：每隔3S发送一次参数和电量
	if( 0 == send_elec_quantity % 100 && SEND_THICK_STATUS == mSendStatus)
	{
		p_serial_communicate->add_send_command_list(SEND_COMMAND_GET_ALL_PARAM);
		p_serial_communicate->add_send_command_list(SEND_COMMAND_ELECTRIC_QT);
	}
	
}

void serial::timeSync()
{

	char buffer[13] = {0};
	memcpy(buffer, "#TMSET", 6);
	memcpy(buffer + 8, "%%%%", 4);
	buffer[12] = '\0';

	uint16_t timestamp = static_cast<uint16_t>(TimeUtils::steadyTimeMillis() % 60000);
	buffer[6] = (timestamp >> 8) & 0xFF;
	buffer[7] = timestamp & 0xFF;
	lastSentTimestamp = timestamp;
	
	qDebug() << "Sending timeSync command:" << buffer << "timestamp:" << timestamp;
	
    
	p_serial_communicate->serial_port.write(buffer,12);
	p_serial_communicate->serial_port.flush();	
	qDebug() << "Waiting Time synchronized .......";	
}			
