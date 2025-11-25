#include "widget.h"
#include "ui_widget.h"
#include <QValueAxis>
#include <QTimer>

#include <QProcess>
#include <QStringList>
#include <QCoreApplication>
//定义数据有效时间
const qint64 DATA_VALID_DURATION_MS = 1000; // 数据有效时间为1000毫秒（1秒）
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
  
    // 连接页面切换信号
    connect(ui->btnMeasure, &QPushButton::clicked, this, &Widget::onBtnMeasureClicked);
    connect(ui->btnCalibrate, &QPushButton::clicked, this, &Widget::onBtnCalibrateClicked);
    connect(ui->btnParam, &QPushButton::clicked, this, &Widget::onBtnParamClicked);
    connect(ui->btn_poweroff, &QPushButton::clicked, this, &Widget::onBtnPoweroffClicked);
    connect(ui->btn_reboot, &QPushButton::clicked, this,&Widget::onBtnRebootClicked);
    connect(ui->btn_connect, &QPushButton::clicked, this, &Widget::onBtnConnectClicked);
    connect(pMeasureForm,&form_measure::sendStatusText, this ,&Widget::onSetStatusTxt);
   
    // 初始化Modbus服务器
    initModbusServer();

    // 默认显示测量页面
    ui->stackedWidget->setCurrentWidget(pMeasureForm);

    // 初始化定时器
    m_thicknessUpdateTimer = new QTimer(this);
    m_thicknessUpdateTimer->setInterval(DATA_VALID_DURATION_MS); // 设置超时时间为1000毫秒（1秒）
    m_thicknessUpdateTimer->setSingleShot(true); // 设置为单次触发模式
      //初始化为未连接状态
    onConnectStatus(false);
    // 初始化新增的重试请求定时器
    m_retryRequestTimer = new QTimer(this);
    m_retryRequestTimer->setInterval(1000); // 设置为1秒
    m_retryRequestTimer->setSingleShot(false); // 设置为重复触发
    connect(m_retryRequestTimer, &QTimer::timeout, this, &Widget::onRetryRequestTimeout);

    // 连接定时器超时信号到我们将要创建的槽函数
    connect(m_thicknessUpdateTimer, &QTimer::timeout, this, &Widget::onThicknessUpdateTimeout);

}

Widget::~Widget()
{
    delete m_thicknessUpdateTimer; // 在析构函数中删除定时器
    delete m_retryRequestTimer; // 删除新定时器
    delete ui;

}
void Widget::onHoldingRegisterChanged(int address, quint16 value)
{
    // 处理保持寄存器的数值变化，并发送信号
    qDebug() <<"onHoldingRegisterChanged"<< "保持寄存器地址" << address << "的值已更改为" << value;

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
    //增加功能： 记录槽函数出发的时间，没出发一次将isConnectStatus置值为true
    //增加功能：超过1S，数据没有更新，槽函数没有被出发，则将isConnectStatus置值为False，厚度值置为0；
    //增加功能：调用onConnectStatus函数，更新连接状态显示
    // 1. 记录当前时间（使用 QDateTime::currentMSecsSinceEpoch() 获取毫秒级时间戳）
    m_lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    //qDebug() << "Thickness updated at:" << m_lastUpdateTime << "ms, Value:" << thickness;

    // 2. 将连接状态置为 true
    if (!isConnectStatus) { // 只有状态发生变化时才更新，避免不必要的UI刷新
        isConnectStatus = true;
        // 调用函数更新连接状态显示
        onConnectStatus(isConnectStatus);
    }

    // 3. 重启定时器。如果在1秒内再次调用此函数，定时器会被重置，不会触发超时
    m_thicknessUpdateTimer->start();
    // 4. 如果重试请求定时器正在运行，说明之前连接丢失，现在已恢复，停止该定时器
    if (m_retryRequestTimer->isActive()) {
        m_retryRequestTimer->stop();
        qDebug() << "重新接收到厚度数据，停止重试请求。";
    }
    // 将厚度数据传递给Modbus服务器
    //qDebug()<<"onThicknessDataChanged"<<thickness;
    if (m_modbusServer && m_modbusServer->isRunning()) {
        m_modbusServer->setThicknessData(isConnectStatus,thickness);
    }
    pMeasureForm->onRecvThhicknesData(thickness);
    pCalibrateForm->on_recv_thickness(thickness);
    
}

// 在 widget.cpp 中添加这个新的槽函数
void Widget::onThicknessUpdateTimeout()
{
    // 定时器超时，意味着超过1秒没有收到新的厚度数据
    if (isConnectStatus) { // 只有当前状态是连接时，才需要处理超时
        isConnectStatus = false;
        qDebug() << "Thickness data timeout. Connection status set to false.";

        // 将厚度值置为0，并更新相关界面和Modbus服务器
        double thickness = 0.0;
        if (m_modbusServer && m_modbusServer->isRunning()) {
            m_modbusServer->setThicknessData(isConnectStatus, thickness);
        }
        pMeasureForm->onRecvThhicknesData(thickness);
        pCalibrateForm->on_recv_thickness(thickness);

        // 调用函数更新连接状态显示
        onConnectStatus(isConnectStatus);   
         // !!! 启动重试请求定时器 !!!
        if (!m_retryRequestTimer->isActive()) {
            m_retryRequestTimer->start();
            qDebug() << "连接超时，开始每隔1秒发送厚度请求。";
        }         
    }
}
void Widget::onRetryRequestTimeout()
{
     //如果当前位于测量页面或者校准页面，发送一次信号，让魔眼启动发送厚度；
    //qDebug() << "发送厚度请求...";
    // 这里是你的厚度请求发送逻辑
    if (ui->stackedWidget->currentWidget() == pMeasureForm ||
        ui->stackedWidget->currentWidget() == pCalibrateForm) {
        emit request_thickness();
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
    //  if (p_timer_get_wave->isActive())
    // {
    //     p_timer_get_wave->stop();
    //     emit send_status_changed(SEND_THICK_STATUS);
    // }
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
