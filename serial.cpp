#include "serial.h"
#include <QThread>
#include "TimeUtils.h"

static INT8 sendCommandList[SEND_COMMAND_LENGTH + 1];

serial::serial(QObject *parent) : QObject(parent)
{
	const auto infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo &info : infos) {
		qDebug() << "Port:" << info.portName();
		qDebug() << "Description:" << info.description();
		qDebug() << "Manufacturer:" << info.manufacturer();
	}
  qDebug()<<"serial info out success";
	serial1.setPortName("ttyAMA0");  // Raspberry Pi OS(Debian Trixie) UART5 port：ttyAMA5，GPIO12(TX)，GPIO13(RX)
	//serial1.setPortName("ttyUSB0");  
	serial1.setBaudRate(QSerialPort::Baud115200);
	serial1.setDataBits(QSerialPort::Data8);
	serial1.setParity(QSerialPort::NoParity);
	serial1.setStopBits(QSerialPort::OneStop);
	serial1.setFlowControl(QSerialPort::NoFlowControl);
	
	QObject::connect(&serial1, &QSerialPort::readyRead,this,&serial::onReadyRead);
	QObject::connect(this, &serial::startTimeSync, this, &serial::timeSync);
	
    

    init_device_param(mDeviceParam);
	timer1 = new QTimer(this);
	timer2 = new QTimer(this);
	timer3 = new QTimer(this);  //同步时钟，如果厚度数据超时则重新同步，timer3每秒触发一次
	timer4 = new QTimer(this);  //获取厚度数据时钟，如果厚度数据开始发送则停止，timer4每100ms触发一次
	connect(timer1, SIGNAL(timeout()), this, SLOT(timer1_slot()));
	connect(timer2, SIGNAL(timeout()), this, SLOT(timer2_slot()));
	connect(timer3, SIGNAL(timeout()), this, SLOT(timeSync()));
	connect(timer4, SIGNAL(timeout()), this, SLOT(getThk()));

}
void serial::ReadData(){
	QByteArray buf = serial1.readAll();
	if(buf[0]=='#'&&buf[1]=='E'&&buf[2]=='Q'&& buf[3]=='T') //Protocol 9:  the header of the charge return frame
		emit _charge(static_cast<int>(buf[4]));
	qDebug()<<"charge: "<<buf[4];

}
void serial::tcpRecData(const QByteArray& buf){
	if(buf[0]=='#'&&buf[9]=='%')
		serial1.write(buf);
	qDebug()<< "serial write:"<<buf;
}
void serial::emitsignal(){
	emit sendthick(thickness_value);
}
void serial::onReadyRead()
{
	int index = 0;
	int index_2 = 0;
	newData = serial1.readAll();
	m_buffer.append(newData);
	qDebug()<<"m_buffer_size: "<< m_buffer.size();
	
	
	index = 0;
	if(((index=m_buffer.indexOf("#TH"))>=0) && m_buffer.size()>=LEN_TH){
		while((index=m_buffer.indexOf("#TH"))>=0){
			t_buffer = m_buffer.mid(index,LEN_TH);
			m_buffer.remove(index,LEN_TH);
			processThicknessData();
		}
	}
	index = m_buffer.indexOf("#TIME");
    if (waitForSyncReply && index >= 0 && m_buffer.size() >= index + 8) 
    {
        ts_buffer = m_buffer.mid(index, 8);
        m_buffer.remove(index, 8);
        
        uint16_t recv_ts = ((uint8_t)ts_buffer[5] << 8) | (uint8_t)ts_buffer[6];
        qDebug() << "recv #TIME frame:" << ts_buffer;
        ts_buffer.clear();
		qDebug() << "[TimeSync] Received ts:" << recv_ts << "expected:" << lastSentTimestamp;
		matched=(recv_ts == lastSentTimestamp);
		if (matched)
		{
			if (timer3->isActive()) 
			{
    			timer3->stop();  // 只有激活时才停止
			}
			qDebug() << "Time synchronized successfully.";
		
			getThk();
			if (!timer4->isActive()) 
			{
    			timer4->start(100);  
			}
			syncInProgress = false;
			waitForSyncReply = false;++
			matched = false;
			
			return ;
		} 
		else

		{
			emit startTimeSync();
			if (!timer3->isActive()) 
			{
    			timer3->start(1000);  
			}
			
		}
		
        
    }
	
	index = 0;
	if(((index=m_buffer.indexOf("#EQT"))>=0) && m_buffer.size()>=LEN_EQT){//&& newData.indexOf(BEGIN_BAG_HEAD)<0
		while((index=m_buffer.indexOf("#EQT"))>=0){
			c_buffer = m_buffer.mid(index,LEN_EQT);
			m_buffer.remove(index,LEN_EQT);
			processChargeData();
		}
	}
	
	index = 0;
//    if(((index=m_buffer.indexOf("#GET"))>=0) && m_buffer.size()>=69){//&& newData.indexOf(BEGIN_BAG_HEAD)<0
//       while((index=m_buffer.indexOf("#EQT"))>=0 && m_buffer[index+68]=='$'){
//           p_buffer = m_buffer.mid(index,69);
//           m_buffer.remove(index,69);
//           emit send_para(p_buffer);
//       }
//      }

	while((index=m_buffer.indexOf("#GET"))>=0 && m_buffer.size()>69){
		index_2 = 0;
		if((index_2=m_buffer.indexOf("#GET", index+1))>=0){
			qDebug()<<index<<index_2;
			if((index_2-index)>0 && (index_2-index)<69){
				m_buffer.remove(index,index_2-index);
				qDebug()<<"hello 1";
			}else{
				qDebug()<<"hello 2";
				p_buffer = m_buffer.mid(index,69);
				qDebug()<<"hello3";
				m_buffer.remove(index,69);
				qDebug()<<"hello4";
                emit send_para(p_buffer);
				qDebug()<<"hello 3";
			}
		} else if(((index_2=m_buffer.indexOf("#GET", index+1))<0) && m_buffer[index+68]=='$'){
			qDebug()<<"hello 2";
			p_buffer = m_buffer.mid(index,69);
			qDebug()<<"hello3";
			m_buffer.remove(index,69);
			qDebug()<<"hello4";
			emit send_para(p_buffer);
		}else{
			break;
		}

	}


	index = 0;
	while(m_buffer.indexOf(BEGIN_BAG_HEAD_30)>=0 && m_buffer[m_buffer.indexOf(WAVE_TAIL)+14]==0xdc && m_buffer[m_buffer.indexOf(WAVE_TAIL)+15]==0xba){
		qDebug()<<"begin remove size:"<<m_buffer.size();
		m_buffer.remove(0,m_buffer.indexOf(BEGIN_BAG_HEAD_01));
		qDebug()<<"messy remove size:"<<m_buffer.size();
		index = 0;
		while((index = m_buffer.indexOf(THICKNESS_HEAD, index))>=0){
			m_buffer.remove(index,LEN_TH);
		}
		qDebug()<<"Thickness removed size"<<m_buffer.size();
		index = 0;
		while((index = m_buffer.indexOf(BEGIN_BAG_HEAD, index))>=0){
			m_buffer.remove(index,3);
		}
		qDebug()<<"begin bag removed size"<<m_buffer.size();
		index = 0;
		while((index = m_buffer.indexOf(END_BAG_TAIL, index))>=0){
			m_buffer.remove(index,2);
		}
		qDebug()<<"end bag removed size"<<m_buffer.size();
		index = 0;
		while((index = m_buffer.indexOf(WAVE_TAIL, index))>=0){
			m_buffer.remove(index+2,14);
			break;
		}
		qDebug()<<"after remove size:"<<m_buffer.size();
		qDebug()<<"success";

		w_buffer = m_buffer.left(sizeof(WAVE_DATA) + sizeof (HEAD_READ_WAVE) + 1 +sizeof(TAIL_READ_WAVE));
		m_buffer.remove(0,sizeof(WAVE_DATA) + sizeof (HEAD_READ_WAVE) + 1 +sizeof(TAIL_READ_WAVE));
	}
	if(w_buffer.startsWith("#ST") && w_buffer[w_buffer.size()-1]==0x55 && w_buffer[w_buffer.size()-2]==0xaa){
		qDebug()<<"w_buffer size:"<<w_buffer.size();
		processWaveData();
	}
	qDebug()<<"wave size:"<<w_buffer.size();
	qDebug()<<"wava data:"<<w_buffer;
	//qDebug()<<"wave tail 1:"<<m_buffer[m_buffer.indexOf(WAVE_TAIL)+12];
	//qDebug()<<"wave tail 2:"<<m_buffer[m_buffer.indexOf(WAVE_TAIL)+13];

	if(m_buffer.size()>8000)
	{
		m_buffer.clear();
	}
	// 处理缓冲区中的数据包

}

void serial::init_device_param(DEVICE_ULTRA_PARAM_U& deviceParam)
{
    deviceParam.stParam.gate2End.index = 0;
    deviceParam.stParam.gate2End.value = 300;

    deviceParam.stParam.zeroPointEnvelope.index = 1;
    deviceParam.stParam.zeroPointEnvelope.value = 2470;

    deviceParam.stParam.ultraSpeed.index = 2;
    deviceParam.stParam.ultraSpeed.value = 32500;

    deviceParam.stParam.zeroPointOrigin.index = 3;
    deviceParam.stParam.zeroPointOrigin.value = 2470;

    deviceParam.stParam.gate2Start.index = 4;
    deviceParam.stParam.gate2Start.value = 200;

    deviceParam.stParam.gate1Start.index = 5;
    deviceParam.stParam.gate1Start.value = 100;

    deviceParam.stParam.gate1End.index = 6;
    deviceParam.stParam.gate1End.value = 180;

    deviceParam.stParam.calibrationThick.index = 7;
    deviceParam.stParam.calibrationThick.value = 1200;

    deviceParam.stParam.temperature.index = 8;
    deviceParam.stParam.temperature.value = 25;

    deviceParam.stParam.curveSmooth.index = 9;
    deviceParam.stParam.curveSmooth.value = USING_CURVE_SMOOTH;

    deviceParam.stParam.smoothTime.index = 10;
    deviceParam.stParam.smoothTime.value = 4;

    deviceParam.stParam.sensorGain.index = 11;
    deviceParam.stParam.sensorGain.value = 300;

    deviceParam.stParam.ferqSelect.index = 12;
    deviceParam.stParam.ferqSelect.value = LOW_FREQUENCY;

    deviceParam.stParam.waveType.index = 13;
    deviceParam.stParam.waveType.value = ORIGIN_WAVE;

    deviceParam.stParam.measureMode.index = 14;
    deviceParam.stParam.measureMode.value = MEASURE_AUTO;

    deviceParam.stParam.SNR.index = 15;
    deviceParam.stParam.SNR.value = 10;

    deviceParam.stParam.dampingRadio.index = 16;
    deviceParam.stParam.dampingRadio.value = 10;

    deviceParam.stParam.envelopeWidth.index = 17;
    deviceParam.stParam.envelopeWidth.value = 2470;

    deviceParam.stParam.widthHeightRadio.index = 18;
    deviceParam.stParam.widthHeightRadio.value = 10;

    deviceParam.stParam.thresholdValue.index = 19;
    deviceParam.stParam.thresholdValue.value = 10;

    deviceParam.stParam.emitInterval.index = 20;
    deviceParam.stParam.emitInterval.value = 1000;

    deviceParam.stParam.pulseNumber.index = 21;
    deviceParam.stParam.pulseNumber.value = 2;

    deviceParam.stParam.excitationFrequency.index = 22;
    deviceParam.stParam.excitationFrequency.value = 500;

    deviceParam.stParam.startDisplay.index = 23;
    deviceParam.stParam.startDisplay.value = 2;

    deviceParam.stParam.stopDisplay.index = 24;
    deviceParam.stParam.stopDisplay.value = 30;

    deviceParam.stParam.communicateMode.index = 25;
    deviceParam.stParam.communicateMode.value = 0;

    deviceParam.stParam.measureControlMode.index = 26;
    deviceParam.stParam.measureControlMode.value = PP_CONTROL_MODE;

    deviceParam.stParam.manualMeasureMode.index = 27;
    deviceParam.stParam.manualMeasureMode.value = MANUAL_PP_CONTROL_MODE;

    deviceParam.stParam.lcdDisplayTxt.index = 28;
    deviceParam.stParam.lcdDisplayTxt.value = DISPLAY_THICKNESS;

    deviceParam.stParam.selfMultiplying.index = 29;
    deviceParam.stParam.selfMultiplying.value = SELF_MULTIPLYING;

    deviceParam.stParam.lcdDisplayTxt.index = 30;
    deviceParam.stParam.thicknessSmooth.value = 30;

    deviceParam.stParam.lcdDisplayTxt.index = 31;
    deviceParam.stParam.thicknessSmooth.value = TITANIUM_ALLOY;
}
void serial::processChargeData()
{
	char c = c_buffer[4];
	emit _charge(static_cast<int>(c));
	qDebug()<<"process charge data: "<< c;
	c_buffer.clear(); // 移除已处理的数据
}

//void serial::processParamData()
//{
//    for (int i = 0; i < PARAM_SZIE; i++)
//    {
//        mDeviceParam.arrParam[i].index = i;
//        mDeviceParam.arrParam[i].value = curRecvData.arrResult[i];
//    }
//}

void serial::processThicknessData()
{
	if (timer4->isActive()) 
	{
    	timer4->stop();  // 只有激活时才停止
	}
	
	if (t_buffer.size() >= LEN_TH && t_buffer[LEN_TH-1]=='$') 
	{
	    /*Packet loss rate test begin*/
	
	    #ifdef __TEST_LOSS_RATE__
		uint8_t num = t_buffer[3];
        uint8_t lossdelta = (uint8_t)(num - testcnt);  // 自动处理回绕
        testcnt = num + 1;  // 记录下一次期望的编号（自动回绕）
		testloss += lossdelta;
		testTotal += (lossdelta==0) ? 1 : lossdelta;
		testRate = 100.0 * testloss / testTotal;
		if(lossdelta != 0)
		{
			qDebug().noquote() << QString("Delta: %1 Total: %2 Loss Rate: %3%")
								  .arg(lossdelta)
								  .arg(testTotal)
								  .arg(testRate, 0, 'f', 2);
			
		}
		

        #endif
		/*Packet loss rate test end*/	
		quint16 thickness = (t_buffer[4] << 8) | t_buffer[5];
		/*	start	name:ash	time:20250703	note:Added operations for processing timestamps,Protocol(5)
			
		*/
        // 静态变量：记录连续超时次数
        static int consecutiveTimeouts = 0;
		if(LEN_TH > 7) // 开O2优化将直接屏蔽该分支判断语句
		{
			uint16_t now = static_cast<uint16_t>(TimeUtils::steadyTimeMillis() % 60000);
			//uint16_t now = 5500;
			uint16_t timestamp = (t_buffer[6] << 8) | t_buffer[7];
			uint16_t delta = now > timestamp ? (now - timestamp) : (now + 60000 - timestamp);
			qDebug()<<"recv th:"<<t_buffer;
			qDebug()<<"now: "<< now << "timestamp: " << timestamp <<"delta: " << delta;
			if(delta > 400)
			{
			    qDebug() << "TimeOut" ;
				consecutiveTimeouts++;
				t_buffer.clear(); // drop stale data
				m_buffer.clear();
				emit _thickness(static_cast<double>(0));//add 20250926
				if (consecutiveTimeouts >= 10) 
				{
					serial1.write("#STOTHK%%%%%");
					serial1.waitForBytesWritten(5);
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					
					if(!syncInProgress)
					{
						emit startTimeSync();  // 触发异步校时
						if (!timer3->isActive()) 
						{
	    					timer3->start(1000);  
						}
						consecutiveTimeouts = 0;  // 触发后立即清零
					}
					
				}
				
				
				return ;
			}
			else
			{
			    consecutiveTimeouts = 0;
			}
			
			//Todo：
		}
			
		/*	end		*/
		
		emit _thickness(static_cast<double>(thickness));

		
		t_buffer.clear(); // 移除已处理的数据
	}
}
short convertToLittleBigEndian(short data)
{
	char a[2];
	short* result;
	char* tmp = (char*)&data;
	a[0] = tmp[1];
	a[1] = tmp[0];

	result = (short*)a;

	return *result;
}

void serial::processWaveData()
{
	WAVE_DATA cur_data = { 0 };
	qDebug()<<"success";
	// 处理波形数据匿        
	if (w_buffer.size() == sizeof(WAVE_DATA) + sizeof (HEAD_READ_WAVE) + 1 +sizeof(TAIL_READ_WAVE)){
		qDebug()<<"success";
		char* buf = w_buffer.data();
		short* p_data = (short*) (&buf[sizeof (HEAD_READ_WAVE) + 1]);
		cur_data.thickness = convertToLittleBigEndian(p_data[0]);
		cur_data.pos_first = convertToLittleBigEndian(p_data[1]);
		cur_data.pos_second = convertToLittleBigEndian(p_data[2]);
		cur_data.gain = convertToLittleBigEndian(p_data[3]);
		cur_data.freq = convertToLittleBigEndian(p_data[4]);
		cur_data.control_mode = convertToLittleBigEndian(p_data[5]);

		qDebug()<<"data trans";
		for(int i = 0; i < WAVE_NUM; i++)
		{
			cur_data.wave_data[i] = convertToLittleBigEndian(p_data[i + 6]);
		}
		qDebug()<<"trans success";

		emit _thickness(static_cast<double>(cur_data.thickness));
		emit _wave(cur_data);
		w_buffer.clear();
	}
}





/*	start	name:ash	time:20250703	note:Protocol(10),
			
*/
void serial::timeSync()
{
	syncInProgress = true;
	m_buffer.clear();
	char buffer[13] = {0};
	memcpy(buffer, "#TMSET", 6);
	memcpy(buffer + 8, "%%%%", 4);
	buffer[12] = '\0';
			
	uint16_t timestamp = static_cast<uint16_t>(TimeUtils::steadyTimeMillis() % 60000);
	buffer[6] = (timestamp >> 8) & 0xFF;
	buffer[7] = timestamp & 0xFF;
	lastSentTimestamp = timestamp;
	ts_buffer.clear();
			
	qDebug() << "Sending timeSync command:" << buffer << "timestamp:" << timestamp;
	//m_buffer.clear();///
	waitForSyncReply = true;
	serial1.write(buffer);
	serial1.waitForBytesWritten(5);	
		
			
	qDebug() << "Waiting Time synchronized .......";
	serial1.waitForReadyRead( 1000);
		
}

		
			
/*	end		*/


void serial::getThk()
{
	serial1.write("#GETTHK%%%%%");
	serial1.waitForBytesWritten(5);
}




void serial::timer1_slot(){
	serial1.write("#GETBAT%%%%%");
	qDebug()<<"send power success";
}
void serial::timer2_slot(){
	serial1.write("#RST$%%%%%%%");
	qDebug()<<"send wave success";
}

void serial::onParamChanged(INT16 param_no, INT16 param_val)
{
	INT16 sendParam[2] = { 0 };
	const INT8 setParamHead[4] = { 0x23, 0x53, 0x45, 0x54 };
	const INT8 setParamtail[4] = { 0x24, 0x25, 0x25, 0x25 };

	sendParam[0] = convertToLittleBigEndian(param_no);
	sendParam[1] = convertToLittleBigEndian(param_val);

	memset(sendCommandList, 0, sizeof(sendCommandList));
	
	//复制头
	memcpy(sendCommandList, setParamHead, sizeof(setParamHead));
	//复制数据
	size_t paramLen = sizeof(sendParam);  
	memcpy(&sendCommandList[sizeof(setParamHead)], sendParam, paramLen);

	// 复制尾
	memcpy(&sendCommandList[sizeof(setParamHead) + paramLen], setParamtail, sizeof(setParamtail));

	serial1.write(reinterpret_cast<const char*>(sendCommandList), SEND_COMMAND_LENGTH);
	
	// ---------------------- 16进制打印 ----------------------
    QString hexStr; 
    for (size_t i = 0; i < SEND_COMMAND_LENGTH; ++i) {
        // 将每个INT8（signed char）转换为无符号字节（避免负数转换错误）
        uint8_t byte = static_cast<uint8_t>(sendCommandList[i]);
        // 格式化为两位16进制（大写），不足两位补0（如0x3 → "03"，0x1A → "1A"）
        hexStr += QString("%1 ").arg(byte, 2, 16, QLatin1Char('0')).toUpper();
    }
    qDebug() << "onParamChanged" << hexStr.trimmed(); 
}

void serial::onReadParam(INT16 param_no){
    serial1.write("#GET00$%%%%%");
    qDebug()<<"send  read_param success";
}


