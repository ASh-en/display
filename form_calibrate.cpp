#include "form_calibrate.h"
#include "ui_form_calibrate.h"

form_calibrate::form_calibrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_calibrate)
{
    ui->setupUi(this);
    pStepWorkActualThick = new std::vector<float> { 2.5, 5.0, 10.0, 20.0 };
    pStepWorkMeasureThick = new std::vector<float> { 0.0, 0.0, 0.0, 0.0 };
    init_tblwidget(*pStepWorkActualThick, *pStepWorkMeasureThick);

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