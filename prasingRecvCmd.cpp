#include "prasingRecvCmd.h"
#include <stdlib.h>
#include <qDebug>
#include "paramDefine.h"

const QString HEART_BEAT_RECV_DATA = "#HEARTBEAT" ;
const QString RECV_THICKNESS_HEAD = "#TH";
const QString RECV_READ_WAVE_HEAD = "#ST" +(char)(0x00);
const QString SET_PARAM_RETRUN_DATA = "#SETOK$"; 
const QString GET_RECV_PARAM_CMD_HEAD ="#GET";
const QString STOP_THICK_RECV_DATA = "#STOOK$";
const QString CELIBRATE_ULTRA_SPEED = "#ONEKEYBDO";
const char RECV_WAVE_TAIL[] = { 0x00, 0xAA };

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
 prasingRecvCmd::prasingRecvCmd()
 {


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
		 index += strnlen_s(head, 12); // Move index forward to avoid finding the same occurrence again
		 if (index > recvBuff.length() - 2)
			 break;
	 }

	 return num;
 }

 int  findNextPos(const QByteArray &recvBuff, const QString &head, int formerPos)
 {
	 int pos = -1;
	 if (0 == recvBuff.length() || 0 == head.size())
	 {
		 return -1;
	 }
	 if (formerPos + head.length() > recvBuff.length() - 1)
	 {
		 return -1;
	 }
	 return recvBuff.indexOf(head, formerPos >= 0 ? formerPos + head.length() : 0);
	/* int startIndex = formerPos >= 0 ? formerPos + head.length() : 0;
	 for (int i = startIndex; i < recvBuff.size() - head.length(); i++)
	 {
		 pos = recvBuff.indexOf(head, i);
		 if (0 <= pos)
		 {
			 return pos;
		 }
	 }*/
	 //return -1;
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
	
 qDebug()<<recvBuff;
	
	if (MIN_DATA_LEN > recvBuff.length())
	{	
		return -1;
	}
	while (0 < func_list.size())
	{
		for (int i = 0; i < func_list.size(); i++)
		{
			func_list[i].pos = findNextPos(recvBuff, func_list[i].head, func_list[i].pos);
		}
		qSort(func_list.begin(), func_list.end(), my_compare);
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
		curRecvData.arrResult[i] = convertToLittleBigEndian(pData[i]);
	}
	//memcpy_s(curRecvData.arrResult, sizeof(curRecvData.arrResult), 
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
	
	const char RECV_READ_WAVE_TAIL[] = { 0xAA, 0x55 };
	INT16* pData = nullptr;

	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	if ((RECV_READ_WAVE_HEAD.length() + 1 + sizeof(RECV_READ_WAVE_TAIL) + (WAVE_POINT_NUM + 6) * sizeof(short))
	> dataLen - pos)
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
	for (int i = 0; i < sizeof(RECV_READ_WAVE_TAIL); i++)
	{
		if (RECV_READ_WAVE_TAIL[i] != recvBuff[pos + RECV_READ_WAVE_HEAD.length() + 1 + (WAVE_POINT_NUM + 6) * sizeof(short) + i])
		{
			return -1;
		}
	}
	curRecvData.cmdType = RECV_COMMAND_READ_WAVE;

	pData = (INT16*)&recvBuff[pos + RECV_READ_WAVE_HEAD.length() + 1];
	for (int i = 0; i < WAVE_POINT_NUM ; i++)
	{
		curRecvData.arrResult[i] = convertToLittleBigEndian(pData[i]);
	}
	return RECV_READ_WAVE_HEAD.length() + sizeof(RECV_READ_WAVE_TAIL) + (WAVE_POINT_NUM + 1) * sizeof(short);
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
	if (sizeof(CELIBRATE_RETRUN_DATA_OK) != strnlen_s(&(recvBuff[pos]),dataLen))
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
int prasingRecvThick(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos)
{
	
	const char RECV_THICKNESS_TAIL[] = { '$' };
	INT16* pData = nullptr;

	if (NULL == recvBuff)
	{
		printf("recvBuff is NULL. %s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	//qDebug() << "prasingRecvThick";
	if ((RECV_THICKNESS_HEAD.length() + sizeof(RECV_THICKNESS_TAIL) + sizeof(char) +sizeof(short)) > dataLen - pos)
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
	if (recvBuff[pos + RECV_THICKNESS_HEAD.length()] != SIGNAL_STATUS_GOOD &&
		recvBuff[pos + RECV_THICKNESS_HEAD.length()] != SIGNAL_STATUS_NOT_BAD &&
		recvBuff[pos + RECV_THICKNESS_HEAD.length()] != SIGNAL_STATUS_BAD &&
		recvBuff[pos + RECV_THICKNESS_HEAD.length()] != SIGNAL_STATUS_NO)
	{
		return -1;
	}
	for (int i = 0; i < sizeof(RECV_THICKNESS_TAIL); i++)
	{
		if (RECV_THICKNESS_TAIL[i] != recvBuff[pos + RECV_THICKNESS_HEAD.length() + sizeof(char) + sizeof(short) + i])
		{
			return -1;
		}
	}
	curRecvData.cmdType = RECV_COMMAND_GET_THICK;
	pData = (INT16*) &recvBuff[pos + sizeof(RECV_THICKNESS_HEAD)];
	
	for (int i = 0; i < 1; i++)
	{
		curRecvData.arrResult[i] = convertToLittleBigEndian(pData[i]);
	}
	curRecvData.recvDataLen = 1;
	return sizeof(RECV_THICKNESS_HEAD) + sizeof(RECV_THICKNESS_TAIL) + sizeof(char) + sizeof(short);
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
