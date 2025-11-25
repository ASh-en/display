#include "prasingRecvCmd.h"
#include <stdlib.h>
#include <QDebug>
#include "param_define.h"
#include "TimeUtils.h"

const QString HEART_BEAT_RECV_DATA = "#HEARTBEAT" ;
const QString RECV_THICKNESS_HEAD = "#TH";
const QString RECV_READ_WAVE_HEAD = "#ST" +(char)(0x00);
const QString SET_PARAM_RETRUN_DATA = "#SETOK$"; 
const QString GET_RECV_PARAM_CMD_HEAD ="#GET";
const QString STOP_THICK_RECV_DATA = "#STOOK$";
const QString CELIBRATE_ULTRA_SPEED = "#ONEKEYBDO";
const char RECV_WAVE_TAIL[] = { 0x00, 0xAA };
const QString RECV_ELECTRIC_QT_HEAD = "#EQT";
const QString RECV_READ_WAVE_TAIL = QString::fromLatin1("\xAA\x55");
const  QString PACKET_WAVE_DATA_BEGIN = QString::fromLatin1("\xAB\xCD");
const QString SYNC_TIME_RESONSE_HEAD = "#TIME";
const int PACKET_SIZE = 3180 / 30;
 short convert2LittleBigEndian(short data)
{
	char a[2];
	short* result;
	char* tmp = (char*)&data;
	a[0] = tmp[1];
	a[1] = tmp[0];

	result = (short*)a;

	return *result;
}
 prasingRecvCmd::prasingRecvCmd()
 {
	 last_pack_num = -1;
	 tmpArray = new QByteArray;

 }
 bool my_compare(FUNC_S a, FUNC_S b)
 {
	 return a.pos < b.pos;
 }
int findAllOccurrences(const QByteArray &recvBuff, const char* head, QList<int>& positions)
 {
	
	 int index = 0;
	 int num = 0;
	 //QByteArray headBytes = head.toUtf8();

	 while ((index = recvBuff.indexOf(head, index)) != -1) {
		 num++;
		 positions.append(index);
		 index += qstrnlen(head, 12); // Move index forward to avoid finding the same occurrence again
		 if (index > recvBuff.length() - 2)
			 break;
	 }

	 return num;
 }

 int  findNextPos(const QByteArray &recvBuff, const QByteArray &head, int formerPos)
 {
	 int pos = -1;
	 if (recvBuff.isEmpty() || head.isEmpty())
	 {
		 return -1;
	 }
	 if (formerPos + head.length() > recvBuff.length() - 1)
	 {
		 return -1;
	 }
	 return recvBuff.indexOf(head, formerPos >= 0 ? formerPos + head.length() : 0);
 }



 

 int prasingRecvCmd::waveDataMatching(QByteArray& recvBuff)
 {
	 const char PACKET_WAVE_DATA_END[] = { 0xDC, 0xBA };
	 int pos = -1;
	 int offset = -1;
	 int packet_number = -1;
	 if (-1 == last_pack_num)
	 {
		 tmpArray->clear();
	 }
	 while (0 <= (pos = findNextPos(recvBuff, PACKET_WAVE_DATA_BEGIN.toLatin1(), -1)))
	 {
		 //qDebug() << pos;
		 if (PACKET_SIZE > recvBuff.length() - pos)
		 {
			 return EOK;
		 }
		 packet_number = static_cast<unsigned char>(recvBuff[pos + PACKET_WAVE_DATA_BEGIN.length()]);
		 if (0 > packet_number || 0x1D < packet_number)
		 {
			 recvBuff.remove(pos, PACKET_SIZE);
			 last_pack_num = -1;
			 tmpArray->clear();
			 break;
		 }
		 offset = pos + PACKET_SIZE - sizeof(PACKET_WAVE_DATA_END);
		 if (recvBuff[offset] == PACKET_WAVE_DATA_END[0] && recvBuff[offset + 1] == PACKET_WAVE_DATA_END[1]) 
		 {
			 // �������Ƿ�����
			 if ( packet_number == last_pack_num + 1) 
			 {
				 last_pack_num = packet_number;
				 QByteArray packetData = recvBuff.mid(pos + PACKET_WAVE_DATA_BEGIN.length() + sizeof(char), PACKET_SIZE -
					 PACKET_WAVE_DATA_BEGIN.length() - sizeof(PACKET_WAVE_DATA_END) - sizeof(char));
				 recvBuff.remove(pos, PACKET_SIZE);
				 
				 //qDebug() << tmpArray->size()<<"Packet" << packet_number << "data:" << packetData;
				 *tmpArray += packetData;
				 if (0x1D == packet_number)
				 {
					 pos = findNextPos(*tmpArray, RECV_READ_WAVE_TAIL.toLatin1(), -1);
					 if (0 < pos)
					 {
						 tmpArray->remove(pos + RECV_READ_WAVE_TAIL.length(), tmpArray->size() - 
							 pos - RECV_READ_WAVE_TAIL.length()+2);
						 recvBuff.append(*tmpArray);
					 }
					 last_pack_num = -1;
					 tmpArray->clear();
				 }
				 
			 }
			 else 
			 {
				 recvBuff.remove(pos, PACKET_SIZE);
				 last_pack_num = -1;
				 tmpArray->clear();
				 break;
			 }
		
		 }
		 else
		 {
			 recvBuff.remove(pos, PACKET_SIZE);
			 last_pack_num = -1;
			 tmpArray->clear();
			 break;
		 }
	 }
	 return EOK;

 }

int prasingRecvCmd::parsingCommand(QByteArray& recvBuff, RECV_PRASING_DATA& curRecvData)
{
	QList<FUNC_S> func_list;
	func_list.append({ -1, STOP_THICK_RECV_DATA, sizeof(STOP_THICK_RECV_DATA),prasingStopThick });
	func_list.append({ -1, HEART_BEAT_RECV_DATA, sizeof(HEART_BEAT_RECV_DATA), prasingHeartBeat });
	func_list.append({ -1, RECV_THICKNESS_HEAD,  sizeof(RECV_THICKNESS_HEAD), prasingRecvThick });
	func_list.append({ -1, RECV_READ_WAVE_HEAD, sizeof(RECV_READ_WAVE_HEAD),prasingReadWave });
	func_list.append({ -1, SET_PARAM_RETRUN_DATA, sizeof(SET_PARAM_RETRUN_DATA), prasingSetParam });
	func_list.append({ -1, GET_RECV_PARAM_CMD_HEAD, sizeof(GET_RECV_PARAM_CMD_HEAD),prasingGetAllParam });
	func_list.append({ -1, CELIBRATE_ULTRA_SPEED, sizeof(CELIBRATE_ULTRA_SPEED),prasingCelibrate });
	func_list.append({ -1, RECV_ELECTRIC_QT_HEAD, RECV_ELECTRIC_QT_HEAD.length(), prasingElectricQuantity });
	func_list.append({ -1, SYNC_TIME_RESONSE_HEAD, sizeof(SYNC_TIME_RESONSE_HEAD), prasingSyncTime });
	
	if (MIN_DATA_LEN > recvBuff.length())
	{	
		return -1;
	}
	waveDataMatching(recvBuff);
	while (0 < func_list.size())
	{
		for (int i = 0; i < func_list.size(); i++)
		{
			func_list[i].pos = findNextPos(recvBuff, func_list[i].head.toLatin1(), func_list[i].pos);
		}
		qSort(func_list.begin(), func_list.end(), my_compare);
		//qDebug() << recvBuff.length() << "data" << recvBuff;
		for (int i = func_list.size() - 1; i >= 0; i--)
		{
			if (-1 == func_list.at(i).pos)
			{
				func_list.removeAt(i);
				continue;
			}
			pFunc prasing_func = func_list.at(i).func;
			int dataLen = prasing_func(recvBuff.data(), recvBuff.length(), curRecvData, recvBuff.indexOf(func_list[i].head));
			if (0 < dataLen)
			{
				recvBuff.remove(recvBuff.indexOf(func_list[i].head), dataLen);
				return EOK;
			}
		}
	}
	return -1;
}
int prasingGetAllParam(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData,int pos)
{
	
	const char GET_RECV_PARAM_CMD_TAIL[] = { 0x24 };
	INT16* pData = nullptr;

	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	if (0 > pos)
	{
		return -1;
	}
	if ((GET_RECV_PARAM_CMD_HEAD.length() + sizeof(GET_RECV_PARAM_CMD_TAIL) + PARAM_SIZE * sizeof(short)) > dataLen - pos)
	{
		return -1;
	}
	qDebug() << "prasingGetAllParam";
	for (int i = 0; i < GET_RECV_PARAM_CMD_HEAD.length(); i++)
	{
		if (GET_RECV_PARAM_CMD_HEAD[i] != recvBuff[pos + i])
		{
			return -1;
		}
	}
	for (int i = 0; i < sizeof(GET_RECV_PARAM_CMD_TAIL); i++)
	{
		if (GET_RECV_PARAM_CMD_TAIL[i] != recvBuff[pos + GET_RECV_PARAM_CMD_HEAD.length() + PARAM_SIZE * sizeof(short) + i])
		{
			return -1;
		}
	}
	curRecvData.cmdType = RECV_COMMAND_GET_ALL_PARAM;
	pData = (INT16*)&recvBuff[pos + GET_RECV_PARAM_CMD_HEAD.length()];
	for (int i = 0; i < PARAM_SIZE; i++)
	{
		curRecvData.arrResult[i] = convert2LittleBigEndian(pData[i]);
	}
	//qMemCopy(curRecvData.arrResult, sizeof(curRecvData.arrResult), 
	//	&recvBuff[sizeof(GET_RECV_PARAM_CMD_HEAD)], PARAM_SIZE * sizeof(short));
	curRecvData.recvDataLen = PARAM_SIZE;
	return GET_RECV_PARAM_CMD_HEAD.length() + sizeof(GET_RECV_PARAM_CMD_TAIL) + PARAM_SIZE * sizeof(short);

}
int prasingSetParam(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos)
{
	
	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	//qDebug() << "prasingSetParam";
	if (SET_PARAM_RETRUN_DATA.length() > dataLen - pos)
	{
		return -1;
	}
	for (int i = 0; i < SET_PARAM_RETRUN_DATA.length(); i++)
	{
		if (recvBuff[pos + i] != SET_PARAM_RETRUN_DATA[i])
		{
			return -1;
		}
	}
	curRecvData.cmdType = RECV_COMMAND_SEND_PARAM;
	curRecvData.recvDataLen = 0;
	curRecvData.rtnStatus = true;
	return sizeof(SET_PARAM_RETRUN_DATA);
}
int prasingReadWave(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData,int pos)
{
	
	
	INT16* pData = nullptr;

	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}

	if ((RECV_READ_WAVE_HEAD.length() + 1 + RECV_READ_WAVE_TAIL.length() + (WAVE_NUM + 6) * sizeof(short))> dataLen - pos)
	{
		return -1;
	}
	
	
	for (int i = 0; i < RECV_READ_WAVE_HEAD.length(); i++)
	{
		if (RECV_READ_WAVE_HEAD[i] != recvBuff[pos + i])
		{
			return -1;
		}
	}
	for (int i = 0; i < RECV_READ_WAVE_TAIL.length(); i++)
	{
		if (RECV_READ_WAVE_TAIL[i].toLatin1() != recvBuff[pos + RECV_READ_WAVE_HEAD.length() + 1 + (WAVE_NUM + 6) * sizeof(short) + i])
		{
			return -1;
		}
	}
	curRecvData.cmdType = RECV_COMMAND_READ_WAVE;

	pData = (INT16*)&recvBuff[pos + RECV_READ_WAVE_HEAD.length() + 1];
	for (int i = 0; i < WAVE_NUM ; i++)
	{
		curRecvData.arrResult[i] = convert2LittleBigEndian(pData[i]);
	}
	return RECV_READ_WAVE_HEAD.length() + RECV_READ_WAVE_TAIL.length() + (WAVE_NUM + 1) * sizeof(short);
}
int prasingCelibrate(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos)
{
	const char CELIBRATE_RETRUN_DATA_OK[] = { 0x23, 0x4f, 0x4e, 0x45, 0x4b, 0x45, 0x59, 0x42, 0x44, 0x4f, 0x4b, 0x24 };
	const char CELIBRATE_RETRUN_DATA_FAIL[] = { 0x23, 0x4f, 0x4e, 0x45, 0x4b, 0x45, 0x59, 0x42, 0x44, 0x4f, 0x46, 0x24 };
	qDebug() << "prasingCelibrate";
	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	if (sizeof(CELIBRATE_RETRUN_DATA_OK) != qstrnlen(&(recvBuff[pos]),dataLen))
	{
		return -1;
	}
	for (int i = 0; i < sizeof(CELIBRATE_RETRUN_DATA_OK) && 
		i < sizeof(CELIBRATE_RETRUN_DATA_FAIL); i++)
	{
		if (recvBuff[pos + i] != CELIBRATE_RETRUN_DATA_OK[i] && 
			recvBuff[pos + i] != CELIBRATE_RETRUN_DATA_FAIL[i])
		{
			return -1;
		}
	}
	curRecvData.cmdType = RECV_COMMAND_CELIBRATE;
	curRecvData.rtnStatus = (CELIBRATE_RETRUN_DATA_OK[10] == recvBuff[pos + 10]) ? true : false;
	return sizeof(CELIBRATE_RETRUN_DATA_OK);
}

enum SIGNAL_STATUS_E
{
	SIGNAL_STATUS_GOOD = 0,
	SIGNAL_STATUS_NOT_BAD = 1,
	SIGNAL_STATUS_BAD = 3,
	SIGNAL_STATUS_NO = 4
};
int prasingElectricQuantity(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos)
{
	const QString ELECTRIC_QT_TAIL = "$$";
	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	qDebug() << "prasingRecvThick";
	if ((RECV_ELECTRIC_QT_HEAD.length() + ELECTRIC_QT_TAIL.length() + sizeof(char)) > dataLen - pos)
	{
		return -1;
	}
	for (int i = 0; i < RECV_ELECTRIC_QT_HEAD.length(); i++)
	{
		if (RECV_ELECTRIC_QT_HEAD[i] != recvBuff[i + pos])
		{
			return -1;
		}
	}
	for (int i = 0; i < ELECTRIC_QT_TAIL.length(); i++)
	{
		if (ELECTRIC_QT_TAIL[i] != recvBuff[pos + RECV_ELECTRIC_QT_HEAD.length() + sizeof(char) + i])
		{
			return -1;
		}
	}
	qDebug() << "prasingRecvThick"<<RECV_COMMAND_ELECTRIC_QT;
	curRecvData.cmdType = RECV_COMMAND_ELECTRIC_QT;
	curRecvData.arrResult[0] = recvBuff[pos + RECV_ELECTRIC_QT_HEAD.length()];
	curRecvData.recvDataLen = 1;
	return  RECV_ELECTRIC_QT_HEAD .length()+ ELECTRIC_QT_TAIL.length() + sizeof(char);

}
int prasingRecvThick(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos)
{
	const char RECV_THICKNESS_TAIL[] = { '$' };
	INT16* pData = nullptr;
	uint8_t count = 0;
    uint16_t timestamp = 0;

	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	//qDebug() << "prasingRecvThick";
	if ((RECV_THICKNESS_HEAD.length() + sizeof(RECV_THICKNESS_TAIL) + sizeof(char) + sizeof(short) + sizeof(short)) > dataLen - pos)
	{
		return -1;
	}
	for (int i = 0; i < RECV_THICKNESS_HEAD.length(); i++)
	{
		if (RECV_THICKNESS_HEAD[i] != recvBuff[i + pos])
		{
			return -1;
		}
	}
	//if (recvBuff[pos + RECV_THICKNESS_HEAD.length()] != SIGNAL_STATUS_GOOD &&
	//	recvBuff[pos + RECV_THICKNESS_HEAD.length()] != SIGNAL_STATUS_NOT_BAD &&
	//	recvBuff[pos + RECV_THICKNESS_HEAD.length()] != SIGNAL_STATUS_BAD &&
	//	recvBuff[pos + RECV_THICKNESS_HEAD.length()] != SIGNAL_STATUS_NO)
	//{
	//	return -1;
	//}
	for (int i = 0; i < sizeof(RECV_THICKNESS_TAIL); i++)
	{
		if (RECV_THICKNESS_TAIL[i] != recvBuff[pos + RECV_THICKNESS_HEAD.length() + sizeof(char) + sizeof(short) + sizeof(short) + i])
		{
			return -1;
		}
	}
	curRecvData.cmdType = RECV_COMMAND_GET_THICK;
	count = static_cast<uint8_t>(recvBuff[pos + RECV_THICKNESS_HEAD.length()]);
    curRecvData.arrResult[0] = count;
	pData = (INT16*) &recvBuff[pos + sizeof(RECV_THICKNESS_HEAD)];
	curRecvData.arrResult[1] = convert2LittleBigEndian(pData[0]);
    
    timestamp = convert2LittleBigEndian(pData[1]);
    curRecvData.arrResult[2] = timestamp;
   
	curRecvData.recvDataLen = 3;
	return sizeof(RECV_THICKNESS_HEAD) + sizeof(RECV_THICKNESS_TAIL) + sizeof(char) + sizeof(short) + sizeof(short);
}
int prasingHeartBeat(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos)
{
	//qDebug() << "prasingHeartBeat";
	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	if (HEART_BEAT_RECV_DATA.length() > dataLen - pos)
	{
		return -1;
	}
	for (int i = 0; i < HEART_BEAT_RECV_DATA.length(); i++)
	{
		if (recvBuff[pos + i] != HEART_BEAT_RECV_DATA[i])
		{
			return -1;
		}
	}
	curRecvData.cmdType = RECV_COMMAND_HEART_BEAT;
	curRecvData.rtnStatus = true;
	curRecvData.recvDataLen = 0;
	return sizeof(HEART_BEAT_RECV_DATA);
}
int prasingStopThick(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos)
{
	//qDebug() << "prasingStopThick";
	
	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	if (STOP_THICK_RECV_DATA.length() > dataLen - pos)
	{
		return -1;
	}
	for (int i = 0; i < STOP_THICK_RECV_DATA.length(); i++)
	{
		if (recvBuff[pos + i] != STOP_THICK_RECV_DATA[i])
		{
			return -1;
		}
	}
	curRecvData.cmdType = RECV_COMMAND_STOP_THICK;
	curRecvData.rtnStatus = true;
	curRecvData.recvDataLen = 0;
	return STOP_THICK_RECV_DATA.length();
}
int prasingSyncTime(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos)
{
    const char RECV_SYNC_TIME_TAIL[] = { '%' };
    uint16_t recv_ts = 0;
	uint16_t* pData = nullptr;

	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	
	if ((SYNC_TIME_RESONSE_HEAD.length() + sizeof(RECV_SYNC_TIME_TAIL) + sizeof(short)) > dataLen - pos)
	{
		return -1;
	}
	for (int i = 0; i < SYNC_TIME_RESONSE_HEAD.length(); i++)
	{
		if (SYNC_TIME_RESONSE_HEAD[i] != recvBuff[i + pos])
		{
			return -1;
		}
	}
	
	for (int i = 0; i < sizeof(RECV_SYNC_TIME_TAIL); i++)
	{
		if (RECV_SYNC_TIME_TAIL[i] != recvBuff[pos + SYNC_TIME_RESONSE_HEAD.length() + sizeof(short) + i])
		{
			return -1;
		}
	}

	curRecvData.cmdType = RECV_COMMAND_SYNC_TIME;
	
    pData = (uint16_t*) &recvBuff[pos + sizeof(SYNC_TIME_RESONSE_HEAD) + 1];
	recv_ts = convert2LittleBigEndian(pData[0]);
    //recv_ts =pData[1]*256+pData[2] ;
	qDebug()<<"recv_ts"<<recv_ts<<"pData0"<<pData[0];
	//qDebug()<<"sizeof"<<sizeof(SYNC_TIME_RESONSE_HEAD)<<((uint8_t*)&recvBuff[pos])[0];
	
    curRecvData.arrResult[0] = recv_ts;
	curRecvData.recvDataLen = 1;
	return sizeof(SYNC_TIME_RESONSE_HEAD) + sizeof(RECV_SYNC_TIME_TAIL) + sizeof(short);

}