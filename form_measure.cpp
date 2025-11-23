#include "form_measure.h"
#include "ui_form_measure.h"
#include <QDebug>
#include <QStringList>


form_measure::form_measure(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_measure)
{
    ui->setupUi(this);
    recvDataNum = 0;
    initMaterialInfo();
    

}

form_measure::~form_measure()
{
    delete ui;
}

/**槽函数，接收电量，根据当前的电量数值给progressBar_elec_quantity控件赋值，如果≥60%就显示绿色，20-60为黄色，≤20为红色 */
void form_measure::onBatteryQuantityChanged(const int& data)
{
    const int BATTERY_SCALE_FACTOR = 1.6;
    // 1. 输入参数有效性检查（避免异常值）
    if (data < 0) {
        qWarning() << "无效的电池电量数据：" << data;
        return;
    }
    // 2. 电量转换与范围限制（0-100）
    int elec_quantity = qBound(0, static_cast<int>(data * BATTERY_SCALE_FACTOR), 100);
    // 3. 更新进度条数值（确保进度条范围正确）
    ui->progressBar_elec_quantity->setRange(0, 100);
    ui->progressBar_elec_quantity->setValue(elec_quantity);

    // 4. 根据电量范围设置进度条颜色（通过样式表）
    QString style;
    if (elec_quantity >= 60) {
        // ≥60%：绿色
        style = "QProgressBar::chunk { background-color: rgb(0, 255, 0); }";  // 纯绿色
    } else if (elec_quantity > 20) {
        // 20%-60%：黄色
        style = "QProgressBar::chunk { background-color: rgb(255, 255, 0); }";  // 纯黄色
    } else {
        // ≤20%：红色
        style = "QProgressBar::chunk { background-color: rgb(255, 0, 0); }";  // 纯红色
    }
    // 应用样式表（设置进度条填充部分的颜色）
    ui->progressBar_elec_quantity->setStyleSheet(style);

}
/**槽函数：接收并显示厚度信息 */
void form_measure::onRecvThhicknesData(const double& data)
{
    QString statusTxt;
    recvDataNum++;
    double elecCurrent = (data*16.0)/20000.0+4.0;//DAC写入值范围为0~65535，对应电流4~20mA，厚度0~20000um
    ui->ldt_thickness->setText(QString::number(data/1000.0, 'f', 3));
    if(elecCurrent>20.0) elecCurrent=20.0;
    ui->ldt_output->setText(QString::number(elecCurrent, 'f', 3));
    _dac.elec_write(elecCurrent);
    
    statusTxt = "num:" + QString::number(recvDataNum)  // 整数转字符串
              + "	th:" + QString::number(data / 1000.0, 'f', 3) + "mm"  // 浮点数转字符串（保留3位小数）
              + "	elec:" + QString::number(elecCurrent, 'f', 3);  // 电流值保留3位小数
    emit sendStatusText(statusTxt);

    emit sendThicknessData(data / 1000.0);
}
void form_measure::initMaterialInfo()
{
    ui->cbx_material->clear();
    m_displayToOriginMaterial.clear();
    QString display_al_name = QString::fromLocal8Bit("铝合金");

    // 获取所有材料的原始名称（如"AL2219"、"AL7050"）
    QStringList allMaterials = m_materialManager.getAllMaterialNames();
    qDebug()<<allMaterials;

    foreach (const QString& originName, allMaterials)
    {
        QString displayName = originName;  // 创建副本
        displayName.replace("AL", display_al_name);  // 在副本上操作
        
        ui->cbx_material->addItem(displayName);
        m_displayToOriginMaterial[displayName] = originName;
    }

    // 2. 获取当前材料及声速，更新界面
    auto currentMatAndSpeed = m_materialManager.getCurrentMaterialAndSpeed();
    QString currentOriginName = currentMatAndSpeed.first;
    double currentSpeed = currentMatAndSpeed.second;

    // 找到当前材料的显示名称，设置为下拉框当前项
    QString currentDisplayName = currentOriginName.replace("AL", display_al_name);
    int index = ui->cbx_material->findText(currentDisplayName);
    if (index != -1) {
        ui->cbx_material->setCurrentIndex(index);
    }

    // 显示当前声速（保留1位小数，单位m/s）
    ui->ldt_ultra_speed->setText(QString::number(currentSpeed, 'f', 1));

    // 连接下拉框变化信号到槽函数
    connect(ui->cbx_material, &QComboBox::currentTextChanged, 
            this, &form_measure::onMaterialChanged);
}
/**写一个槽函数，当cbx_material变化时，根据当前的材料信息，显示不同的声速 */
void form_measure::onMaterialChanged(const QString& displayName)
{
     // 1. 根据显示名称获取原始材料名称（如"铝合金7050" -> "AL7050"）
    if (!m_displayToOriginMaterial.contains(displayName)) {
        return; // 无效名称，直接返回
    }
    QString originName = m_displayToOriginMaterial[displayName];

    // 2. 根据原始名称获取声速
    double speed = m_materialManager.getSpeedByMaterial(originName);
    if (speed <= 0) {
        ui->ldt_ultra_speed->setText("无效");
        return;
    }
    // 3. 显示声速（保留1位小数）
    ui->ldt_ultra_speed->setText(QString::number(speed, 'f', 1));

    //声速的参数号02
    INT16 param_no = 0x02;
    INT16 param_value = (int) (speed * 10);

    emit sendParamChanged(param_no, param_value);

}



