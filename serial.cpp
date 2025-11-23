#include "serial.h"
#include <QThread>
#include "TimeUtils.h"
#include <QtEndian> // 需要包含这个头文件
#include "param_define.h"



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
	serial1.setPortName("ttyAMA5");  // Raspberry Pi OS(Debian Trixie) UART5 port：ttyAMA5，GPIO12(TX)，GPIO13(RX) ttyAMA0
	//serial1.setPortName("ttyUSB0");  
	serial1.setBaudRate(QSerialPort::Baud115200);
	serial1.setDataBits(QSerialPort::Data8);
	serial1.setParity(QSerialPort::NoParity);
	serial1.setStopBits(QSerialPort::OneStop);
	serial1.setFlowControl(QSerialPort::NoFlowControl);
	
    timertick = new QTimer(this);  //基准时间步进 5ms

	QObject::connect(&serial1, &QSerialPort::readyRead,this,&serial::onReadyRead);
	QObject::connect(this, &serial::startTimeSync, this, &serial::timeSync);
    QObject::connect(this, &serial::getCharge, this, &serial::timer_elec_quantity_slot);
    QObject::connect(timertick, &QTimer::timeout, this, &serial::StateCheck);
	//默认开启接收厚度
    
    init_device_param(mDeviceParam);
	
    timertick->start(5); //5ms


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
    while(((index=m_buffer.indexOf("#STOOK$"))>=0))
    {
		m_buffer.remove(index,7);
        emit _thickness(static_cast<double>(0)); //厚度数据清零，防止结束发送后，电流输出仍为最后一个接收到的厚度数据
        isRecvThicknessData = false;
        waitForStopThkResponse = false;
        qDebug()<<"Received STOOK";
	}
	index = 0;
	if(((index=m_buffer.indexOf("#TH"))>=0) && m_buffer.size()>=LEN_TH)
    {
		while((index=m_buffer.indexOf("#TH"))>=0)
        {
			t_buffer = m_buffer.mid(index,LEN_TH);
			m_buffer.remove(index,LEN_TH);

			processThicknessData(index);
		}
	}

	index = 0;
	//开启接收厚度数据，并且正在等待同步回复，并且缓冲区中有完整的时间同步帧
    if (((index=m_buffer.indexOf("#TIME"))>=0) && m_buffer.size() >= index + 8) 
    {
        ts_buffer = m_buffer.mid(index, 8);
        m_buffer.remove(index, 8);

        processTimeSync();
        
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
	const int PARAM_BUFFER_LENGTH = PARAM_SIZE *sizeof(INT16) + 5;
	while((index=m_buffer.indexOf("#GET"))>=0 && m_buffer.size()> PARAM_BUFFER_LENGTH){
		index_2 = 0;
		if((index_2=m_buffer.indexOf("#GET", index+1))>=0){
			qDebug()<<index<<index_2;
			if((index_2-index)>0 && (index_2-index)<PARAM_BUFFER_LENGTH){
				m_buffer.remove(index,index_2-index);
				
			}else{
				p_buffer = m_buffer.mid(index,PARAM_BUFFER_LENGTH);
				m_buffer.remove(index,PARAM_BUFFER_LENGTH);
                //emit send_para(p_buffer);
				processParamData(p_buffer);
			}
		} else if(((index_2=m_buffer.indexOf("#GET", index+1))<0) && m_buffer[index+68]=='$'){
			
			p_buffer = m_buffer.mid(index,PARAM_BUFFER_LENGTH);
			
			m_buffer.remove(index,PARAM_BUFFER_LENGTH);
			processParamData(p_buffer);
			//emit send_para(p_buffer);
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
	//qDebug()<<"wave size:"<<w_buffer.size();
	//qDebug()<<"wava data:"<<w_buffer;
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

	//参数项与现有协议不符合，所以先注释掉
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

void serial::processChargeData()
{
	char c = c_buffer[4];
	emit _charge(static_cast<int>(c));
	qDebug()<<"process charge data: "<< c;
	c_buffer.clear(); // 移除已处理的数据
}

void serial::processParamData(const QByteArray &para_data)
{
    if (para_data.size() < sizeof(DEVICE_PARAM_S) / sizeof(PARAM_STRUCT_S) * 2) {
        qDebug() << "参数数据长度不足，无法解析。";
        return;
    }

    const char *dataPtr = para_data.constData();

    for (int i = 0; i < PARAM_SIZE; i++) {
        // qFromBigEndian 会从指定地址读取 sizeof(T) 个字节，并转换为本地字节序
        quint16 value = qFromBigEndian<quint16>(dataPtr + i * 2);
        mDeviceParam.arrParam[i].index = i;
        mDeviceParam.arrParam[i].value = value;
    }
	emit send_dev_params(mDeviceParam);

}

void serial::processThicknessData(int index)
{
	noThkResponse = false;
	
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
            m_buffer.remove(0, index);  //Todo:丢弃当前数据包之前的数据，可能会导致其他数据（主要为电量数据）被遗弃,需要进一步优化
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
			//qDebug()<<"recv th:"<<t_buffer;
			qDebug()<<"now: "<< now << "timestamp: " << timestamp <<"delta: " << delta;
            emit _delayUpdated(delta);
			if(delta > 400)
			{
			    qDebug() << "TimeOut" ;
				consecutiveTimeouts++;
				t_buffer.clear();                           // drop stale data
				m_buffer.clear();
				emit _thickness(static_cast<double>(0));    //add 20250926
				if (consecutiveTimeouts >= 10) 
				{
                    consecutiveTimeouts = 0;        // 触发后立即清零
                    if(!waitForStopThkResponse)
                    {
                        stopThk();                      // 停止获取厚度数据
                    }
                    
                    matched = false;
					if(!isInTimeSyncProgress)
					{
						emit startTimeSync();       // 触发异步校时
					}
					
				}		
				return ;
			}
			else
			{
			    consecutiveTimeouts = 0;            // 非连续重置超时计数器
			}
			
			//Todo：
		}
			
		/*	end		*/	
		emit _thickness(static_cast<double>(thickness));
		t_buffer.clear(); // 移除已处理的数据
	}
}

void serial::processTimeSync()
{
    // 解析接收到的时间戳
    uint16_t recv_ts = (static_cast<uint8_t>(ts_buffer[5]) << 8) |
                       static_cast<uint8_t>(ts_buffer[6]);

    qDebug() << "recv #TIME frame:" << ts_buffer;
    qDebug() << "[TimeSync] Received ts:" << recv_ts
             << "expected:" << lastSentTimestamp;

    matched = (recv_ts == lastSentTimestamp);

    if (matched)
    {
        isInTimeSyncProgress = false;
        qDebug() << "Time synchronized successfully.";
        // 获取厚度数据
        getThk();
        // 重置同步状态
    }
    else
    {
        // 时间戳不匹配，重新触发校时
        qDebug() << "Time synchronized failed. Retrying...";
        emit startTimeSync();
    }
    ts_buffer.clear(); // 移除已处理的数据
}



void serial::processWaveData()
{
	WAVE_DATA cur_data = { 0 };
	qDebug()<<"processWaveData called";
	// 处理波形数据匿        
	if (w_buffer.size() == sizeof(WAVE_DATA) + sizeof (HEAD_READ_WAVE) + 1 +sizeof(TAIL_READ_WAVE)){
		qDebug()<<"process wave data";
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
	isInTimeSyncProgress = true;
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
    
	serial1.write(buffer);
	serial1.flush();	
	qDebug() << "Waiting Time synchronized .......";	
}

		
			
/*	end		*/


void serial::getThk()
{
    isRecvThicknessData = true;
    noThkResponse = true;
    
    emit _thickness(static_cast<double>(0));    // 清零厚度数据。
                                                // 若出现异常情况（如魔眼断电）导致仍停留在“接收厚度数据”状态，
                                                // 但实际未再收到任何新的厚度数据时，StateCheck() 会调用本函数，
                                                // 以确保最后一次接收到的厚度数据不会长期保持为有效状态。
    serial1.write("#GETTHK%%%%%");
	serial1.flush();
    qDebug()<<"send get thick success";
}

void serial::stopThk()
{
    
	serial1.write("#STOTHK%%%%%");
    serial1.flush();
    waitForStopThkResponse = true;
	qDebug()<<"send stop thick success";
    //返回  #STOOK$

}


void serial::timer_elec_quantity_slot()
{
	serial1.write("#GETBAT%%%%%");
    serial1.flush();
	qDebug()<<"send power success";
    //返回  #EQTx%%
}

void serial::on_timer_get_wave_slot(){
    stopThk();//Todo:需要确认使用情况
	serial1.write("#RST$%%%%%%%");
    serial1.flush();
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

void serial::onReadParam(){
    serial1.write("#GET00$%%%%%");
    serial1.flush();
    qDebug()<<"send  read_param success";
}


void serial::StateCheck()
{
    
    // qDebug()<<"StateCheck timer tick";
    if(timecount % 200 == 0)//1s
    {
       qDebug()<<waitForStopThkResponse<<isInTimeSyncProgress<<isRecvThicknessData<<noThkResponse;
    }
    
    if(timecount % 2000 == 0) //10s
    {
        emit getCharge();
    }
    if((timecount % 100 == 0) && waitForStopThkResponse && isRecvThicknessData) //500ms
    {
        stopThk();
    }
    if((timecount % 100 == 0) && isInTimeSyncProgress)  //500ms
    {
        emit startTimeSync();     //连接按钮，可使用该函数,如果matched标志位为true，则为连接状态，
    }
    if((timecount % 50 == 0) && (!waitForStopThkResponse) && isRecvThicknessData) //250ms 
    {
        if(noThkResponse)getThk();  //在接收厚度数据的过程中，如果没有收到厚度数据，则重新发送获取厚度命令，processThicknessData中收到数据后将noThkResponse置为false
        else noThkResponse = true;  //重置标志，保证由于其他原因没有收到数据时可重复发送获取厚度命令，正常情况下processThicknessData在250ms内会收到数据并将noThkResponse置为false，不会重复发送获取厚度命令
                                    //可修改为判断当前时间与上次收到厚度数据时间的差值是否超过250ms，该判断需要增加一个变量记录上次收到厚度数据的时间
    }
    

    timecount = (timecount + 1) % 40000;
    
}
