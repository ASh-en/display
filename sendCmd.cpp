#include "sendCmd.h"
#include <stdlib.h>
#include "stdio.h"
#include <qdebug.h>
//#GETTHK%%%%%

char* sendCmd::getSendCommand(SEND_COMMAND_NUMBER_E commandNum, INT8* param, int paramLen)
{
	const INT8 setParamHead[4] = { 0x23, 0x53, 0x45, 0x54 };
	const INT8 setParamtail[4] = { 0x24, 0x25, 0x25, 0x25 };
	const INT8 celiHead[10] = { 0x23, 0x4F, 0x4E, 0x45, 0x4B, 0x45, 0x59, 0x42, 0x44, 0x25 };

	memset(sendCommandList, 0, sizeof(sendCommandList));
	switch (commandNum)
	{
	case SEND_COMMAND_GET_ALL_PARAM:
		//INT8 tmp[COMMAND_LENGTH + 1] = ;//{ 23, 47, 45, 54, 30 , 30, 24, 25, 25, 25, 25, 25 };
		memcpy(sendCommandList, "#GET00$%%%%%", qstrnlen("#GET00$%%%%%", COMMAND_LENGTH));
		return (char*)sendCommandList;
		break;
	case SEND_COMMAND_SEND_PARAM:
		if (NULL == param && 4 != paramLen)
		{
			printf("[error] param is wrong\n");
			return NULL;
		}
		memcpy(sendCommandList, setParamHead, sizeof(setParamHead));
		memcpy(&sendCommandList[sizeof(setParamHead)], param, paramLen);
		memcpy(&sendCommandList[sizeof(setParamHead) + paramLen],setParamtail, sizeof(setParamtail));
		break;
	case SEND_COMMAND_READ_WAVE:

		memcpy(sendCommandList,  "#RST$%%%%%%%",
			qstrnlen("#RST$%%%%%%%", COMMAND_LENGTH));
		break;
	case SEND_COMMAND_CELIBRATE:
		if (NULL == param && 2 != paramLen)
		{
			printf("[error] param is wrong\n");
			return NULL;
		}

		memcpy(sendCommandList, celiHead, sizeof(celiHead));
		memcpy(&sendCommandList[sizeof(celiHead)],  param, paramLen);
		break;
	case SEND_COMMAND_GET_THICK:
		memcpy(sendCommandList,  "#GETTHK%%%%%", qstrnlen("#GETTHK%%%%%", COMMAND_LENGTH));
		break;
	case SEND_COMMAND_STOP_THICK:
		memcpy(sendCommandList,  "#STOTHK%%%%%", qstrnlen("#STOTHK%%%%%", COMMAND_LENGTH));
		break;
	case SEND_COMMAND_ELECTRIC_QT:
		memcpy(sendCommandList, "#GETBAT%%%%%", qstrnlen("#GETBAT%%%%%", COMMAND_LENGTH));
		break;
	default:
		return NULL;
		break;
	}
	return (char*)sendCommandList;
}
