#include "form_calibrate.h"
#include "ui_form_calibrate.h"
#include <QDebug>
#include <QMessageBox>

form_calibrate::form_calibrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_calibrate)
{
    ui->setupUi(this);
    init_tblwidget(pStepWorkActualThick, pStepWorkMeasureThick);
    QObject::connect(ui->ptn_calculate, &QPushButton::clicked, this, &form_calibrate::ptn_clicked_calculate);
    QObject::connect(ui->ptn_save_speed, &QPushButton::clicked, this, &form_calibrate::on_btn_save_actual_data);
    QObject::connect(ui->ptn_set_speed, &QPushButton::clicked, this, &form_calibrate::on_set_ultra_speed);

    initMaterialInfo();

	
}

form_calibrate::~form_calibrate()
{
    delete ui;
}


void form_calibrate::init_tblwidget(const std::vector<float>& actualData, const std::vector<float>& measureData)
{
    ui->tbl_step_workpiece->setColumnCount(4);
    ui->tbl_step_workpiece->setRowCount(2);

    // 关键：设置列宽自适应（平均分配控件宽度）
    QHeaderView *horizontalHeader = ui->tbl_step_workpiece->horizontalHeader();
    // 所有列平均拉伸，填充整个控件宽度
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
    // 可选：行高自适应内容（如果内容高度不固定）
    QHeaderView *verticalHeader = ui->tbl_step_workpiece->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 设置表头（原有代码保留）
    QStringList horizontalHeaders = { QString::fromLocal8Bit("阶梯1"),QString::fromLocal8Bit("阶梯2"), QString::fromLocal8Bit("阶梯3"), QString::fromLocal8Bit("阶梯4") };
    ui->tbl_step_workpiece->setHorizontalHeaderLabels(horizontalHeaders);
    ui->tbl_step_workpiece->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

    QStringList verticalHeaders = { QString::fromLocal8Bit("真实数据"), QString::fromLocal8Bit("测量数据") };
    ui->tbl_step_workpiece->setVerticalHeaderLabels(verticalHeaders);
    ui->tbl_step_workpiece->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

    // 插入数据
    for (int i = 0; i < actualData.size() && i < 4; ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(actualData[i], 'f', 2));
        ui->tbl_step_workpiece->setItem(0, i, item);
    }

    for (int i = 0; i < measureData.size() && i < 4; ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(measureData[i], 'f', 3));
        ui->tbl_step_workpiece->setItem(1, i, item);
    }

    // 设置 TableWidget 填充父容器
    ui->tbl_step_workpiece->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void form_calibrate::ptn_clicked_calculate()
{

	std::vector<double> actualThickList, measureThickList;
	int stepNum=0;
	for(int i = 0; i < pStepWorkMeasureThick.size(); ++i)
	{
		if(pStepWorkMeasureThick[i] > 0.0)
		{
			++stepNum;
		}
	}
	for (int col = 0; col < stepNum; ++col)
	{
		QTableWidgetItem *item = ui->tbl_step_workpiece->item(0, col);
		actualThickList.push_back(item->text().toFloat());
	}
	for (int col = 0; col < stepNum; ++col)
	{
		QTableWidgetItem *item = ui->tbl_step_workpiece->item(1, col);
		measureThickList.push_back(item->text().toFloat());
	}
	float cal_speed = calculateBestSpeed(measureThickList, actualThickList, ui->ldt_ultra_speed->text().toDouble());
	ui->ldt_ultra_speed->setText(QString::number(cal_speed, 'f', 1));
}
void form_calibrate::on_set_ultra_speed()
{
    //qDebug()<<"on_set_ultra_speed";
    // 1. 从行编辑框读取文本并转换为数值
    bool ok = false;
    double cal_speed = ui->ldt_ultra_speed->text().toDouble(&ok);

    if (!ok) {
        qDebug() << "错误：无法将输入的速度值转换为数字！";
        return;
    }
    // 2. 获取当前选中的材料名称
    QString cur_material = ui->cbx_material->currentText();
    if (cur_material.isEmpty()) {
        qDebug() << "警告：未选择任何材料，无法保存速度！";
        QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请先选择一种材料！"));
        return;
    }
    
    // 3. 发送参数更改的信号
    emit sendParamChanged(static_cast<INT16>(2), static_cast<INT16>(cal_speed * 10));

    // 4. 根据材料名称保存速度
    m_materialManager.setSpeedByMaterial(cur_material, cal_speed);
    

    qDebug() << "成功设置材料 '" << cur_material << "' 的速度为: " << cal_speed;
}

double form_calibrate::calculateBestSpeed(const std::vector<double>& measuredThicknesses, const std::vector<double>& actualThicknesses, double init_speed)
{
	double sumXY = 0.0;
	double sumX2 = 0.0;

	for (size_t i = 0; i < measuredThicknesses.size(); ++i) {
		double timeDiff = measuredThicknesses[i] / init_speed * 2; // 使用初始速度计算时间差
		sumXY += actualThicknesses[i] * timeDiff;
		sumX2 += timeDiff * timeDiff;
	}

	return sumXY / sumX2 * 2;
}


void form_calibrate::on_recv_thickness(const double& data)
{
    ui->ldt_cur_thickness->setText(QString::number(data/1000.0, 'f', 3));
    current_thickness = data /1000.0 ;
}

//补充函数实现：将厚度保存至表格控件
void form_calibrate::on_btn_save_actual_data()
{
    //qDebug()<<"on_btn_save_actual_data";
    // 1. 获取表格的当前光标索引
    QModelIndex currentIndex = ui->tbl_step_workpiece->currentIndex();

    // 2. 确定目标保存列（测量数据行是第 1 行）
    int targetColumn = 0;
    if (currentIndex.isValid() && currentIndex.row() == 1) { // 光标在测量数据行
        targetColumn = currentIndex.column();
    }

    // 3. 将 current_thickness 转换为测量数据格式（保留 3 位小数）
    QString thicknessText = QString::number(current_thickness, 'f', 3);
    QTableWidgetItem *targetItem = new QTableWidgetItem(thicknessText);

    // 4. 将数据设置到测量数据行（第 1 行）的目标列
    ui->tbl_step_workpiece->setItem(1, targetColumn, targetItem);

    // 5. 光标自动跳转到测量数据行的下一个单元格
    int nextColumn = targetColumn + 1;
    int totalColumns = ui->tbl_step_workpiece->columnCount();
    if (nextColumn >= totalColumns) {
        nextColumn = 0;
    }
    ui->tbl_step_workpiece->setCurrentCell(1, nextColumn); // 保持在测量数据行
}

void form_calibrate::initMaterialInfo()
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
            this, &form_calibrate::onMaterialChanged);
}
/**写一个槽函数，当cbx_material变化时，根据当前的材料信息，显示不同的声速 */
void form_calibrate::onMaterialChanged(const QString& displayName)
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
}