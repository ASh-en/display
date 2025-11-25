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

    // connect new plus/minus buttons and rate combobox
    connect(ui->cbx_rate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &form_calibrate::on_cbx_rate_changed);

    // sensible default step rate (0.1)
    ui->cbx_rate->setCurrentIndex(3);

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

    // 1. 获取表格的列数，这是循环的正确次数
    int columnCount = ui->tbl_step_workpiece->columnCount();

    // 2. 遍历表格的每一列，建立数据对应关系并进行过滤
    for (int col = 0; col < columnCount; ++col)
    {
        // 尝试获取当前列第0行（实际厚度）的单元格
        QTableWidgetItem *actualItem = ui->tbl_step_workpiece->item(0, col);
        // 尝试获取当前列第1行（测量厚度）的单元格
        QTableWidgetItem *measureItem = ui->tbl_step_workpiece->item(1, col);

        // 健壮性检查：确保两个单元格都存在且不为空
        if (!actualItem || !measureItem || actualItem->text().isEmpty() || measureItem->text().isEmpty()) {
            qDebug() << "[ptn_clicked_calculate] 表格第" << col << "列数据不完整或为空，已跳过。";
            continue; // 如果任一单元格无效，则跳过当前列
        }

        // 将单元格文本转换为数值
        bool actualOk, measureOk;
        double actualThick = actualItem->text().toDouble(&actualOk);
        double measureThick = measureItem->text().toDouble(&measureOk);

        // 健壮性检查：确保转换成功且测量厚度有效
        if (actualOk && measureOk && measureThick >= 0.1) {
            // 数据有效，添加到列表中
            actualThickList.push_back(actualThick);
            measureThickList.push_back(measureThick);
        } else {
            // 输出调试信息，帮助定位问题数据
            if (!actualOk) qDebug() << "[ptn_clicked_calculate] 表格第" << col << "列实际厚度值无效。";
            if (!measureOk) qDebug() << "[ptn_clicked_calculate] 表格第" << col << "列测量厚度值无效。";
            if (measureThick < 0.1) qDebug() << "[ptn_clicked_calculate] 表格第" << col << "列测量厚度(" << measureThick << ")小于0.1mm，已跳过。";
        }
    }

    // 3. 检查是否有足够的有效数据来进行计算
    if (actualThickList.empty() || measureThickList.empty() || actualThickList.size() != measureThickList.size()) {
        qDebug() << "[ptn_clicked_calculate] 有效数据不足，无法进行计算。";
        // 可以在这里给用户一个更友好的提示，例如弹出一个消息框
        // QMessageBox::warning(this, "计算失败", "有效数据不足或数据不匹配，无法进行速度计算。");
        return;
    }

    qDebug() << "[ptn_clicked_calculate] 共收集到" << actualThickList.size() << "组有效数据用于计算。";

    // 4. 获取当前速度值
    bool speedOk;
    double currentSpeed = ui->ldt_ultra_speed->text().toDouble(&speedOk);
    if (!speedOk) {
        qDebug() << "[ptn_clicked_calculate] 当前速度值无效。";
        // QMessageBox::warning(this, "计算失败", "当前速度值无效，请检查输入。");
        return;
    }

    // 5. 调用计算函数
    double cal_speed = calculateBestSpeed(measureThickList, actualThickList, currentSpeed);

    // 6. 将计算结果更新到UI
    // 使用 double spinbox：保留一位小数精度
    ui->dsb_cal_speed->setValue(cal_speed);

    qDebug() << "[ptn_clicked_calculate] 计算完成，最佳速度已更新为:" << cal_speed;
}
void form_calibrate::on_set_ultra_speed()
{
    //qDebug()<<"on_set_ultra_speed";
    // 1. 从行编辑框读取文本并转换为数值
    // QDoubleSpinBox 直接返回数值，不需要 text->toDouble 验证
    double cal_speed = ui->dsb_cal_speed->value();
    // 2. 获取当前选中的材料名称
   // 将显示名映射为原始名
    QString cur_material = ui->cbx_material->currentText();
    QString originMaterial = cur_material;
    if (m_displayToOriginMaterial.contains(cur_material)) {
        originMaterial = m_displayToOriginMaterial[cur_material];
    } else {
        qDebug() << "警告：未找到材料显示名" << cur_material << "对应的原始名，使用显示名进行保存。";
    }

    // 3. 发送参数更改的信号
    emit sendParamChanged(static_cast<INT16>(2), static_cast<INT16>(cal_speed * 10));

    // 4. 根据材料名称保存速度
    m_materialManager.setSpeedByMaterial(cur_material, cal_speed);
    

    qDebug() << "成功设置材料 '" << cur_material << "' 的速度为: " << cal_speed;
}

// Increase current focused table cell by selected rate
void form_calibrate::on_ptn_plus_number_clicked()
{
    QModelIndex idx = ui->tbl_step_workpiece->currentIndex();
    if (!idx.isValid()) {
        qDebug() << "[on_ptn_plus_number_clicked] 没有选中的单元格";
        return;
    }
    int row = idx.row();
    int col = idx.column();
    QTableWidgetItem *it = ui->tbl_step_workpiece->item(row, col);
    if (!it) {
        it = new QTableWidgetItem("0");
        ui->tbl_step_workpiece->setItem(row, col, it);
    }
    bool ok;
    double val = it->text().toDouble(&ok);
    if (!ok) val = 0.0;
    double step = ui->cbx_rate->currentText().toDouble(&ok);
    if (!ok) step = 0.1;
    val += step;
    int decimals = (row == 1) ? 3 : 2;
    it->setText(QString::number(val, 'f', decimals));
}

// Decrease current focused table cell by selected rate
void form_calibrate::on_ptn_minus_number_clicked()
{
    QModelIndex idx = ui->tbl_step_workpiece->currentIndex();
    if (!idx.isValid()) {
        qDebug() << "[on_ptn_minus_number_clicked] 没有选中的单元格";
        return;
    }
    int row = idx.row();
    int col = idx.column();
    QTableWidgetItem *it = ui->tbl_step_workpiece->item(row, col);
    if (!it) {
        it = new QTableWidgetItem("0");
        ui->tbl_step_workpiece->setItem(row, col, it);
    }
    bool ok;
    double val = it->text().toDouble(&ok);
    if (!ok) val = 0.0;
    double step = ui->cbx_rate->currentText().toDouble(&ok);
    if (!ok) step = 0.1;
    val -= step;
    int decimals = (row == 1) ? 3 : 2;
    it->setText(QString::number(val, 'f', decimals));
}

void form_calibrate::on_cbx_rate_changed(int index)
{
    Q_UNUSED(index);
    qDebug() << "rate changed to" << ui->cbx_rate->currentText();
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
    ui->dsb_cal_speed->setValue(currentSpeed);

}

void form_calibrate::updateDeviceParams(const DEVICE_ULTRA_PARAM_U &params)
{
    INT16 ultra_speed_number = 0x02;
    ui->ldt_ultra_speed->setText(QString::number(params.arrParam[ultra_speed_number].value / 10.0 , 'f', 1));
    return;
} 
