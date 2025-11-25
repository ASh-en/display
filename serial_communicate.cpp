#include "serial_communicate.h"
#include <QDebug>
#include <qmessagebox.h>
#include "g_var.h"
#include <qmutex.h>

#include <pthread.h>
#include <unistd.h>

static QMutex g_locker;
static QList<SEND_COMMAND_NODE> g_send_cmd_lst;

serial_communicate::serial_communicate()
{
    initPort();
    for (int i = 0; i < sizeof(continue_send_cmd) / sizeof(continue_send_cmd[0]); i++)
    {
        continue_send_cmd[i] = false;
    }
}

void serial_communicate::initPort() 
{
    baud_list << "1200" << "2400" << "4800" << "9600"
              << "14400" << "19200" << "38400" << "56000"
              << "57600" << "115200";
    serial_port.setPortName("ttyAMA5"); // Linux 下串口号
    serial_port.setBaudRate(QSerialPort::Baud115200);
    serial_port.setDataBits(QSerialPort::Data8);
    serial_port.setParity(QSerialPort::NoParity);
    serial_port.setStopBits(QSerialPort::OneStop);
    serial_port.setFlowControl(QSerialPort::NoFlowControl);
}

bool serial_communicate::open_serial_port()
{
    pthread_t thread_id;
    if (serial_port.isOpen())
    {
        return false;
    }
    if (!serial_port.open(QIODevice::ReadWrite))
    {
        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), 
                            QString::fromLocal8Bit("打开串口错误"), QMessageBox::Yes);
        return false;
    }
    // 创建线程
    if (pthread_create(&thread_id, NULL, send_data_proc, this) != 0)
    {
        qDebug() << "创建线程失败";
        return false;
    }
    // 可选：分离线程，避免内存泄漏
    pthread_detach(thread_id);
    return true;
}

int serial_communicate::write_data_to_com(SEND_COMMAND_NODE& send_data)
{
    char* buff = NULL;
    if (!serial_port.isOpen())
    {
        return -1;
    }
    if (NULL == send_data.param)
    {
        buff = mSendCommand.getSendCommand(send_data.commandNum);
    }
    else
    {
        buff = mSendCommand.getSendCommand(send_data.commandNum, send_data.param, send_data.paramLen);
        //qDebug()<<"getSendCommand"<<buff;
    }
    if (0 > serial_port.write(buff, COMMAND_LENGTH))
    {
        qDebug() << QString::fromLocal8Bit("写入数据失败");
        return -1;
    }
    if (!serial_port.waitForBytesWritten(3000))
    {
        qDebug() << QString::fromLocal8Bit("发送数据失败");
        serial_port.clearError();
        return -1;
    }
    return EOK;
}

void serial_communicate::close_serial_port()
{
    if (!serial_port.isOpen())
    {
        return;
    }
    serial_port.clear();
    serial_port.close();
}

// 线程函数
void* send_data_proc(void* pParam)
{
    serial_communicate* pSerial = (serial_communicate*)pParam;
    if (NULL == pSerial)
    {
        return NULL;
    }

    while (pSerial->serial_port.isOpen())
    {
        if (0 == g_send_cmd_lst.size())
        {
            usleep(20 * 1000); // 休眠20ms
            continue;
        }
        if(0 == g_send_cmd_lst.at(0).sendNumber)
        { 
            if (EOK == pSerial->write_data_to_com(g_send_cmd_lst[0]))
            {
                continue_send_cmd[g_send_cmd_lst.at(0).commandNum] = true;
                g_send_cmd_lst[0].sendNumber += 1;
                usleep(500 * 1000); // 休眠50ms
            }
            else
            {
                usleep(20 * 1000);
            }
            continue;
        }
        else if (10 < g_send_cmd_lst[0].sendNumber)
        {
            qDebug() << QString::fromLocal8Bit("连续发送错误");
            g_locker.lock();
            g_send_cmd_lst.pop_front();
            g_locker.unlock();
            continue;
        }

        if (false == continue_send_cmd[g_send_cmd_lst.at(0).commandNum])
        {
            g_locker.lock();
            g_send_cmd_lst.pop_front();
            g_locker.unlock();
        }
        else {
            if (EOK == pSerial->write_data_to_com(g_send_cmd_lst[0]))
            {
                g_locker.lock();
                g_send_cmd_lst[0].sendNumber += 1;
                g_locker.unlock();
            }
            usleep(500 * 1000);
            continue;
        }
    }
    return NULL;
}

int serial_communicate::add_send_command_list(SEND_COMMAND_NUMBER_E commandNum, INT8* param, int paramLen)
{
    SEND_COMMAND_NODE tmpNode = {
    static_cast<SEND_COMMAND_NUMBER_E>(0),  // 枚举成员显式转换
        {0},                                    // 数组成员初始化
        0,                                      // paramLen
        0                                       // sendNumber
    };
    tmpNode.commandNum = commandNum;
    if (NULL != param)
    {
        memcpy(tmpNode.param, param, paramLen);
    }
    tmpNode.paramLen = paramLen;
    tmpNode.sendNumber = 0;

    if (!g_locker.tryLock())
    {
        return -1;
    }
    g_send_cmd_lst.append(tmpNode);
    g_locker.unlock();
    return EOK;
}

void serial_communicate::clear_g_send_cmd_lst()
{
    g_locker.lock();
    g_send_cmd_lst.clear();
    g_locker.unlock();
}
