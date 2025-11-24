#include "widget.h"
#include "ui_widget.h"
#include <QValueAxis>
#include <QTimer>

#include <QProcess>
#include <QStringList>
#include <QCoreApplication>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    showFullScreen();
    pCalibrateForm = new form_calibrate();
    pParamForm = new form_param();
    pMeasureForm = new form_measure();
    ui->stackedWidget->addWidget(pCalibrateForm);
    ui->stackedWidget->addWidget(pParamForm);
    ui->stackedWidget->addWidget(pMeasureForm);
    //初始化为未连接状态
    onConnectStatus(false);
    // 连接页面切换信号
    connect(ui->btnMeasure, &QPushButton::clicked, this, &Widget::onBtnMeasureClicked);
    connect(ui->btnCalibrate, &QPushButton::clicked, this, &Widget::onBtnCalibrateClicked);
    connect(ui->btnParam, &QPushButton::clicked, this, &Widget::onBtnParamClicked);
    connect(ui->btn_poweroff, &QPushButton::clicked, this, &Widget::onBtnPoweroffClicked);
    connect(ui->btn_reboot, &QPushButton::clicked, this,&Widget::onBtnRebootClicked);
    connect(ui->btn_connect, &QPushButton::clicked, this, &Widget::onBtnConnectClicked);
    connect(pMeasureForm,&form_measure::sendStatusText, this ,&Widget::onSetStatusTxt);
    p_timer_get_wave = new QTimer(this);

    // 连接厚度数据信号到Modbus服务器
    connect(pMeasureForm, &form_measure::sendThicknessData, this, &Widget::onThicknessDataChanged); 
    // 初始化Modbus服务器
    initModbusServer();

    // 默认显示测量页面
    ui->stackedWidget->setCurrentWidget(pMeasureForm);

}

Widget::~Widget()
{
    delete ui;

}
void Widget::onHoldingRegisterChanged(int address, quint16 value)
{
    // 处理保持寄存器的数值变化，并发送信号
    qDebug() << "保持寄存器地址" << address << "的值已更改为" << value;

    // 发出信号通知参数变化
    emit paramChanged((INT16)address, value);
}

void Widget::initModbusServer()
{
    m_modbusServer = new ModbusServer(this);
    
    // 启动Modbus TCP服务器，监听所有地址，端口502
    if (!m_modbusServer->startServer(QHostAddress::Any, 502)) 
    {
        qDebug() << "Modbus服务器初始化失败";
    } else {
        qDebug() << "Modbus服务器初始化成功";
    }
    
    connect(m_modbusServer, &ModbusServer::holdingRegisterChanged, 
    this, &Widget::onHoldingRegisterChanged);
    
}


void Widget::onThicknessDataChanged(const double& thickness)
{
    // 将厚度数据传递给Modbus服务器
    if (m_modbusServer && m_modbusServer->isRunning()) {
        m_modbusServer->setThicknessData(isConnectStatus,thickness);
    }
}

void Widget::onBtnMeasureClicked()
{
    resetBtnCheckedState();
    ui->btnMeasure->setChecked(true);
    ui->stackedWidget->setCurrentWidget(pMeasureForm);
    emit send_status_changed(SEND_THICK_STATUS);
    
}

void Widget::onBtnCalibrateClicked()
{
    resetBtnCheckedState();
    ui->btnCalibrate->setChecked(true);
    ui->stackedWidget->setCurrentWidget(pCalibrateForm);
    emit send_status_changed(SEND_THICK_STATUS);

    
}

void Widget::onBtnParamClicked()
{
    resetBtnCheckedState();
    ui->btnParam->setChecked(true);
    ui->stackedWidget->setCurrentWidget(pParamForm);
    emit send_status_changed(NOT_SEND_THICK_STAUS);
}

void Widget::resetBtnCheckedState()
{
    ui->btnMeasure->setChecked(false);
    ui->btnCalibrate->setChecked(false);
    ui->btnParam->setChecked(false);
}

//控制树莓派进行关机
void Widget::onBtnPoweroffClicked()
{
    QProcess::startDetached("sudo shutdown -h now");
}

// 控制树莓派重启（同时重启当前程序）
void Widget::onBtnRebootClicked()
{
    // 1. 获取当前程序的绝对路径（无需写死路径）
    QString appPath = QCoreApplication::applicationFilePath();
    qDebug() << "当前程序路径：" << appPath;

    // 2. 查看串口 ttyAMA5 的占用进程（获取 PID）
    QProcess lsofProcess;
    lsofProcess.start("sudo systemctl restart start-display.service");
    if (!lsofProcess.waitForFinished(5000)) {
        qDebug() << "获取串口占用进程失败：" << lsofProcess.errorString();
        return;
    }

    // // 解析 lsof 输出，提取 PID
    // QString output = lsofProcess.readAllStandardOutput();
    // QString error = lsofProcess.readAllStandardError();
    // if (!error.isEmpty()) {
    //     qDebug() << "lsof 命令错误：" << error;
    // }

    // QStringList lines = output.split("\n", QString::SkipEmptyParts);
    // QList<int> pids;
    // if (lines.size() > 1) { // 跳过标题行
    //     for (int i = 1; i < lines.size(); ++i) {
    //         QStringList parts = lines[i].trimmed().split(QRegExp("\\s+"), QString::SkipEmptyParts);
    //         if (parts.size() >= 2) {
    //             bool ok;
    //             int pid = parts[1].toInt(&ok);
    //             if (ok) {
    //                 pids.append(pid);
    //                 qDebug() << "发现占用 ttyAMA5 的进程 PID：" << pid;
    //             }
    //         }
    //     }
    // }
    // if (pids.isEmpty()) {
    //     qDebug() << "未发现占用 ttyAMA5 的进程";
    // }

    // // 3. 启动新的程序实例（用获取到的路径）
    // bool isStarted = QProcess::startDetached(appPath);
    // if (!isStarted) {
    //     qDebug() << "启动新程序失败！路径：" << appPath;
    //     return;
    // }
    // qDebug() << "新程序已启动：" << appPath;

    // // 4. 终止所有占用串口的旧进程（包括当前程序）
    // foreach (int pid, pids) {
    //     QProcess killProcess;
    //     killProcess.start("sudo kill -9 " + QString::number(pid));
    //     if (killProcess.waitForFinished(2000)) {
    //         qDebug() << "已终止进程 PID：" << pid;
    //     } else {
    //         qDebug() << "终止进程 PID 失败：" << pid << "错误：" << killProcess.errorString();
    //     }
    // }
    // // 主动退出当前程序（可选，kill -9 已强制终止）
    // qApp->quit();
}
/**槽函数：重新连接串口 */
 void Widget::onBtnConnectClicked()
 {
     if (p_timer_get_wave->isActive())
    {
        p_timer_get_wave->stop();
        emit send_status_changed(SEND_THICK_STATUS);
    }
 }


 void Widget::onSetStatusTxt(const QString& txt)
 {
    ui->lbl_status->setText(txt);
 }

/**槽函数：  如果isConnect为true, lbl_connect_status赋值为“已连接”，
 * 控件为绿色，反之，为“未连接”，控件为红色*/
void Widget::onConnectStatus(bool isConnect)
{ 
    isConnectStatus = isConnect;
    QLabel* statusLabel = ui->lbl_connect_status;
    if (!statusLabel) 
    {
        return;
    }
    if (isConnectStatus) 
    {
        //已连接：文本为“已连接”，颜色设为绿色
        statusLabel->setText(QString::fromLocal8Bit("已连接"));
        //方案1：设置文字颜色为绿色（推荐，仅文字变色）
        statusLabel->setStyleSheet("color: rgb(0, 255, 0);");  // 绿色
    } 
    else 
    {
        //未连接：文本为“未连接”，颜色设为红色
        statusLabel->setText(QString::fromLocal8Bit("未连接"));
        statusLabel->setStyleSheet("color: rgb(255, 0, 0);");  // 红色
    }
}
