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

void Widget::initModbusServer()
{
    m_modbusServer = new ModbusServer(this);
    
    // 启动Modbus TCP服务器，监听所有地址，端口502
    if (!m_modbusServer->startServer("192.168.0.1", 502)) {
        qDebug() << "Modbus服务器初始化失败";
    } else {
        qDebug() << "Modbus服务器初始化成功";
    }
}


void Widget::onThicknessDataChanged(const double& thickness)
{
    // 将厚度数据传递给Modbus服务器
    if (m_modbusServer && m_modbusServer->isRunning()) {
        m_modbusServer->setThicknessData(thickness);
    }
}

void Widget::update_para(const QByteArray &para_data){

}

void Widget::onBtnMeasureClicked()
{
    resetBtnCheckedState();
    ui->btnMeasure->setChecked(true);
    ui->stackedWidget->setCurrentWidget(pMeasureForm);
}

void Widget::onBtnCalibrateClicked()
{
    resetBtnCheckedState();
    ui->btnCalibrate->setChecked(true);
    ui->stackedWidget->setCurrentWidget(pCalibrateForm);
}

void Widget::onBtnParamClicked()
{
    resetBtnCheckedState();
    ui->btnParam->setChecked(true);
    ui->stackedWidget->setCurrentWidget(pParamForm);
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

//控制树莓派进行重启
void Widget::onBtnRebootClicked()
{
    // 1. 查看串口ttyAMA0的占用进程，获取PID（进程ID）
    QProcess lsofProcess;
    // 使用lsof命令查看占用/dev/ttyAMA0的进程（需sudo权限）
    lsofProcess.start("sudo lsof /dev/ttyAMA0");
    // 等待命令执行完成（超时5秒）
    if (!lsofProcess.waitForFinished(5000)) {
        qDebug() << "获取串口占用进程失败：" << lsofProcess.errorString();
        return;
    }

    // 读取命令输出（标准输出和错误输出）
    QString output = lsofProcess.readAllStandardOutput();
    QString error = lsofProcess.readAllStandardError();
    if (!error.isEmpty()) {
        qDebug() << "lsof命令错误输出：" << error;
    }

    // 解析输出，提取PID（lsof输出格式：COMMAND  PID USER ...）
    QStringList lines = output.split("\n", QString::SkipEmptyParts);
    QList<int> pids; // 存储占用串口的进程ID
    if (lines.size() > 1) { // 第一行为标题行，跳过
        for (int i = 1; i < lines.size(); ++i) {
            QString line = lines[i].trimmed();
            // 按空格分割行内容（多个空格视为一个分隔符）
            QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            if (parts.size() >= 2) { // 确保有足够的字段
                bool ok;
                int pid = parts[1].toInt(&ok); // 第二列是PID
                if (ok) {
                    pids.append(pid);
                    qDebug() << "发现占用ttyAMA0的进程PID：" << pid;
                }
            }
        }
    }

    if (pids.isEmpty()) {
        qDebug() << "未发现占用ttyAMA0的进程";
    }

    // 2. 启动新的软件实例（重启前先启动新程序，避免kill后无法启动）
    QString appPath = "/home/pi/qt/display/display"; // 软件路径
    bool isStarted = QProcess::startDetached(appPath); // 独立启动新进程
    if (!isStarted) {
        qDebug() << "启动新软件失败！路径：" << appPath;
        return;
    }
    qDebug() << "新软件已启动：" << appPath;

    // 3. 杀掉所有占用串口的旧进程（包括当前程序自身）
    foreach (int pid, pids) {
        QProcess killProcess;
        killProcess.start("sudo kill -9 " + QString::number(pid)); // -9强制终止
        if (killProcess.waitForFinished(2000)) { // 等待2秒
            qDebug() << "已终止进程PID：" << pid;
        } else {
            qDebug() << "终止进程PID失败：" << pid << "错误：" << killProcess.errorString();
        }
    }
    // 主动退出当前程序（可选，因为kill -9已强制终止）
    qApp->quit();
}
/**槽函数：重新连接串口 */
 void Widget::onBtnConnectClicked()
 {
    
 }


 void Widget::onSetStatusTxt(const QString& txt)
 {
    ui->lbl_status->setText(txt);
 }

/**槽函数：  如果isConnect为true, lbl_connect_status赋值为“已连接”，
 * 控件为绿色，反之，为“未连接”，控件为红色*/
void Widget::onConnectStatus(bool isConnect)
{ 
    QLabel* statusLabel = ui->lbl_connect_status;
    if (!statusLabel) 
    {
        return;
    }
    if (isConnect) 
    {
        //已连接：文本为“已连接”，颜色设为绿色
        statusLabel->setText(QString::fromLocal8Bit("未连接"));
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