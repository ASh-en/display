#include "form_calibrate.h"
#include "ui_form_calibrate.h"

form_calibrate::form_calibrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_calibrate)
{
    ui->setupUi(this);
    init_tblwidget(pStepWorkActualThick, pStepWorkMeasureThick);

    connect(ui->ptn_set_speed, &QPushButton::clicked, this, &form_calibrate::on_btn_calibrate_clicked);
}

form_calibrate::~form_calibrate()
{
    delete ui;
}


void form_calibrate::init_tblwidget(const std::vector<float>& actualData, const std::vector<float>& measureData)
{
	ui->tbl_step_workpiece->setColumnCount(4);
	int width = ui->tbl_step_workpiece->width();
	ui->tbl_step_workpiece->setColumnWidth(4, (width / 4));
	ui->tbl_step_workpiece->setRowCount(2);
	// 设置水平表头
	QStringList horizontalHeaders = { QString::fromLocal8Bit("阶梯1"),QString::fromLocal8Bit("阶梯2"), QString::fromLocal8Bit("阶梯3"), QString::fromLocal8Bit("阶梯4") };
	ui->tbl_step_workpiece->setHorizontalHeaderLabels(horizontalHeaders);
	ui->tbl_step_workpiece->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	// 设置垂直表头
	QStringList verticalHeaders = { QString::fromLocal8Bit("真实数据"), QString::fromLocal8Bit("测量数据") };
	ui->tbl_step_workpiece->setVerticalHeaderLabels(verticalHeaders);
	ui->tbl_step_workpiece->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	// 插入实际数据
	for (int i = 0; i < actualData.size(); ++i)
	{
		QTableWidgetItem *item = new QTableWidgetItem(QString::number(actualData[i], 'f', 2));
		ui->tbl_step_workpiece->setItem(0, i, item);
	}

	// 插入测量数据
	for (int i = 0; i < measureData.size(); ++i) {
		QTableWidgetItem *item = new QTableWidgetItem(QString::number(measureData[i], 'f', 3));
		ui->tbl_step_workpiece->setItem(1, i, item);
	}

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
    emit sendParamChanged(2, (int) (cal_speed * 10));
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