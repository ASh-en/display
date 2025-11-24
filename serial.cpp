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
	// INT16 sendParam[2] = { 0 };
	// const INT8 setParamHead[4] = { 0x23, 0x53, 0x45, 0x54 };
	// const INT8 setParamtail[4] = { 0x24, 0x25, 0x25, 0x25 };

	// sendParam[0] = convertToLittleBigEndian(param_no);
	// sendParam[1] = convertToLittleBigEndian(param_val);

	// memset(sendCommandList, 0, sizeof(sendCommandList));
	
	// //复制头
	// memcpy(sendCommandList, setParamHead, sizeof(setParamHead));
	// //复制数据
	// size_t paramLen = sizeof(sendParam);  
	// memcpy(&sendCommandList[sizeof(setParamHead)], sendParam, paramLen);

	// // 复制尾
	// memcpy(&sendCommandList[sizeof(setParamHead) + paramLen], setParamtail, sizeof(setParamtail));

	// serial1.write(reinterpret_cast<const char*>(sendCommandList), SEND_COMMAND_LENGTH);
	
	// // ---------------------- 16进制打印 ----------------------
    // QString hexStr; 
    // for (size_t i = 0; i < SEND_COMMAND_LENGTH; ++i) {
    //     // 将每个INT8（signed char）转换为无符号字节（避免负数转换错误）
    //     uint8_t byte = static_cast<uint8_t>(sendCommandList[i]);
    //     // 格式化为两位16进制（大写），不足两位补0（如0x3 → "03"，0x1A → "1A"）
    //     hexStr += QString("%1 ").arg(byte, 2, 16, QLatin1Char('0')).toUpper();
    // }
    // qDebug() << "onParamChanged" << hexStr.trimmed(); 
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
		qDebug() << curRecvData.cmdType;
		prasing_error_num = 0;
		continue_send_cmd[curRecvData.cmdType] = false;
		switch (curRecvData.cmdType)
		{
		case RECV_COMMAND_SEND_PARAM:
			//set_status_txt(QString::fromLocal8Bit("设置成功"));
			break;
		case RECV_COMMAND_GET_ALL_PARAM:
			//set_status_txt(QString::fromLocal8Bit("获取全部参数成功"));
			for (int i = 0; i < PARAM_SIZE; i++)
			{
				mDiviceParam.arrParam[i].index = i;
				mDiviceParam.arrParam[i].value = curRecvData.arrResult[i];
			}
			emit send_dev_params(mDiviceParam);
			
			break;
		case RECV_COMMAND_GET_THICK:
			recvThickNum++;
			emit _thickness(static_cast<UINT16>(curRecvData.arrResult[0]) / 1000.0);
			// ui->ldt_thick->setText(QString::number(static_cast<UINT16>(curRecvData.arrResult[0]) / 1000.0, 'f', 3));
			//set_status_txt(QString::fromLocal8Bit("读取厚度成功[") + QString::number(recvThickNum) + "]");
			break;
		case RECV_COMMAND_READ_WAVE:
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
		case RECV_COMMAND_ELECTRIC_QT:
			const int SCALE_FACTOR = 1.6;
			emit _charge(static_cast<UINT16>(curRecvData.arrResult[0] * SCALE_FACTOR));
			break;
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

}

void serial::stopThk()
{

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
	if(0 == send_wave_count % 30 && NOT_SEND_THICK_STAUS == mSendStatus)
	{
		p_serial_communicate->add_send_command_list(SEND_COMMAND_READ_WAVE);
		p_serial_communicate->add_send_command_list(SEND_COMMAND_GET_ALL_PARAM);
	}
	//测量状态：每隔3S发送一次参数和电量
	if( 0 == send_elec_quantity % 30 && SEND_THICK_STATUS == mSendStatus)
	{
		p_serial_communicate->add_send_command_list(SEND_COMMAND_GET_ALL_PARAM);
		p_serial_communicate->add_send_command_list(SEND_COMMAND_ELECTRIC_QT);
	}
	
}