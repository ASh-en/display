#pragma once
#include "type_define.h"
#include <QMap>

const int MIN_DATA_LEN = 6;
enum RECV_COMMAND_NUMBER_E
{
	RECV_COMMAND_GET_ALL_PARAM = 1,
	RECV_COMMAND_SEND_PARAM = 2,
	RECV_COMMAND_READ_WAVE = 3,
	RECV_COMMAND_CELIBRATE = 4,
	RECV_COMMAND_GET_THICK = 5,
	RECV_COMMAND_STOP_THICK = 6,
	RECV_COMMAND_HEART_BEAT = 7,
	RECV_COMMAND_ELECTRIC_QT = 8,
};
struct RECV_PRASING_DATA
{
	RECV_COMMAND_NUMBER_E cmdType;
	INT16 arrResult[3000 + 1];
	UINT16 recvDataLen;
	bool rtnStatus;
};
typedef  int(*pFunc)(char*, int, RECV_PRASING_DATA&, int);
struct FUNC_S {
	int pos;
	QString head;
	int len_head;
	pFunc func;
};

short convert2LittleBigEndian(short data);
class prasingRecvCmd
{

public:
	
	int parsingCommand(QByteArray& recvBuff, RECV_PRASING_DATA& curRecvData);
	prasingRecvCmd();
private:
	int last_pack_num;
	QByteArray* tmpArray;
	int waveDataMatching(QByteArray& recvBuff);
	
	

};
int prasingGetAllParam(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData,int pos = 0);
int prasingSetParam(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos = 0);
int prasingReadWave(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos = 0);
int prasingCelibrate(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos = 0);
int prasingRecvThick(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos = 0);
int prasingStopThick(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos = 0);
int prasingHeartBeat(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos = 0);
int prasingElectricQuantity(char* recvBuff, int dataLen, RECV_PRASING_DATA& curRecvData, int pos = 0);
bool my_compare(FUNC_S a, FUNC_S b);


