#pragma once
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <sendCmd.h>

typedef struct
{
    SEND_COMMAND_NUMBER_E commandNum;
    INT8 param[COMMAND_LENGTH];
    int paramLen;
    int sendNumber; //发送次数
}SEND_COMMAND_NODE;

class serial_communicate
{
private:
    void initPort();
    sendCmd mSendCommand;
public:
    QStringList port_name;
    QStringList baud_list;   //波特率
    serial_communicate();
    QSerialPort serial_port;
    bool open_serial_port();
    void close_serial_port();
    int write_data_to_com(SEND_COMMAND_NODE& send_data);
    int add_send_command_list(SEND_COMMAND_NUMBER_E commandNum, INT8* param = NULL, int paramLen = 0);
};

// 线程函数声明
void* send_data_proc(void* pParam);