#include "form_param.h"
#include "ui_form_param.h"


form_param::form_param(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_param)
{
    ui->setupUi(this);
    initPlotGraph();
   set_signal_connect_slot();
}

form_param::~form_param()
{
    delete ui;
}

void form_param::set_signal_connect_slot()
{
//    connect(ui->ptn_connect, &QPushButton::clicked, this, &ultra_param::ptn_clicked_connect_slots);

//    connect(ui->ptn_disconnect, &QPushButton::clicked, this, &ultra_param::ptn_clicked_disconnect_slots);

    connect(ui->ptn_get_param, &QPushButton::clicked, this, &form_param::ptn_clicked_send_param_slots);
    connect(ui->ptn_set_param, &QPushButton::clicked, this, &form_param::ptn_clicked_read_param_slots);
//    connect(ui->ptn_send_ex_param, &QPushButton::clicked, this, &ultra_param::ptn_clicked_send_ex_param_slots);
//    connect(ui->ptn_start_thickness, &QPushButton::clicked, this, &ultra_param::ptn_start_thickness_slots);
//    connect(ui->ptn_stop_thickness, &QPushButton::clicked, this, &ultra_param::ptn_stop_thickness_slots);
//    connect(ui->ptn_wave_get, &QPushButton::clicked, this, &ultra_param::ptn_wave_get_slots);
//    connect(ui->ptn_wave_save, &QPushButton::clicked, this, &ultra_param::on_ptn_wave_save);
//    connect(pTimer, &QTimer::timeout, this, &ultra_param::timer_timeout_slots);

}

void  form_param::initPlotGraph()
{
    ui->widget->addGraph();
    ui->widget->setBackground(QBrush(QColor(144, 238, 144)));
    ui->widget->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    QPen pen;
    pen.setColor(Qt::red); // 红色
    ui->widget->graph(0)->setPen(pen);
    ui->widget->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    //ui->widget->xAxis->setLabel(QString::fromLocal8Bit("时间(μs)"));
    //ui->widget->yAxis->setLabel(QString::fromLocal8Bit("增益"));
    ui->widget->xAxis->setRange(5, 20); // 初始x轴范围
    ui->widget->yAxis->setRange(-6000, 6000); // 初始y轴范围

    QPen pen_line;
    ui->widget->addGraph();
    pen_line.setWidth(3);  //线宽
    pen_line.setColor(Qt::blue);  //线颜色
    pen_line.setStyle(Qt::PenStyle::SolidLine);   //线的类型或风格
    ui->widget->graph(1)->setPen(pen_line);

    QVector<double> x(2), y(2), x1(2), y1(2);
    x[0] = 10;
    x[1] = 10;
    y[0] = 5000;
    y[1] = 6000;
    ui->widget->graph(1)->setData(x, y);

    ui->widget->addGraph();
    pen_line.setColor(Qt::red);  //线颜色
    ui->widget->graph(2)->setPen(pen_line);
    x1[0] = 15;
    x1[1] = 15;
    y1[0] = 5000;
    y1[1] = 6000;
    ui->widget->graph(2)->setData(x1, y1);

    textLabel = new QCPItemText(ui->widget);
    textLabel->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);//文字布局：顶、左对齐
    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例为单位/实际坐标为单位）
    textLabel->position->setCoords(0.80, 0); //把文字框放在X轴的中间，Y轴的最顶部
    textLabel->setText(QString::fromLocal8Bit("增益:20db\n") + QString::fromLocal8Bit("频率:4Mhz\n") + QString::fromLocal8Bit("固定PP"));
    textLabel->setFont(QFont(font().family(), 6)); //字体大小
    //textLabel->setPen(QPen(Qt::red)); //字体颜色
    textLabel->setPadding(QMargins(2, 2, 2, 2));//文字距离边框几个像素

    pTimer = new QTimer(this);

}


void form_param::updatePlotGraph(const WAVE_DATA& waveData)
{
	// QVector<double> time, amp;
	// QVector<double> x(2), y(2), x1(2), y1(2);
	// QString displayTxt;
	// for (int i = 0; i < waveData.pt_vec.size(); i++)
	// {
	// 	time.append(waveData.pt_vec[i].time);
	// 	amp.append(waveData.pt_vec[i].amp);
	// }
	// ui->widget->graph(0)->setData(time, amp);

	// ui->widget->xAxis->setRange(time.first(), time.last());
	// ui->widget->yAxis->setRange( -30000, 30000 /**std::min_element(amp.begin(), amp.end()) * 1.1,
	// 	*std::max_element(amp.begin(), amp.end()) * 1.1*/);
	
	// x[0] = waveData.wave_pos_first;
	// x[1] = waveData.wave_pos_first;
	// y[0] = 28000/**std::max_element(amp.begin(), amp.end())*/;
	// y[1] = 30000/**std::max_element(amp.begin(), amp.end())*1.15*/;
	// ui->widget->graph(1)->setData(x, y);

	// x1[0] = waveData.wave_pos_second;
	// x1[1] = waveData.wave_pos_second;
	// y1[0] = 28000/**std::max_element(amp.begin(), amp.end())*/;
	// y1[1] = 30000/**std::max_element(amp.begin(), amp.end()) *  1.15*/;
	// ui->widget->graph(2)->setData(x1, y1);

	// displayTxt += (QString::fromLocal8Bit("增益:"));
	// displayTxt += (QString::number(waveData.curGain, 'f', 1));
	// displayTxt += "db\n";
	// displayTxt += QString::fromLocal8Bit("频率:");
	// displayTxt += QString::number(waveData.excitation_freq, 'f', 1);
	// displayTxt += "Mhz\n";
	// if (PP_CONTROL_MODE == waveData.measureControlMode)
	// {
	// 	displayTxt += QString::fromLocal8Bit("固定PP");
	// }
	// else {
	// 	displayTxt += QString::fromLocal8Bit("自动");
	// }
	// textLabel->setText(displayTxt);

	// ui->widget->replot();
 }

void form_param::onRecvWaveData(const WAVE_DATA& cur_data)
{
    QVector<double> time, amp;
	QVector<double> x(2), y(2), x1(2), y1(2);
	QString displayTxt;
	for (int i = 0; i < waveData.pt_vec.size(); i++)
	{
		time.append(waveData.pt_vec[i].time);
		amp.append(waveData.pt_vec[i].amp);
	}
	ui->widget->graph(0)->setData(time, amp);

	ui->widget->xAxis->setRange(time.first(), time.last());
	ui->widget->yAxis->setRange( -30000, 30000 /**std::min_element(amp.begin(), amp.end()) * 1.1,
		*std::max_element(amp.begin(), amp.end()) * 1.1*/);
	
	x[0] = waveData.wave_pos_first;
	x[1] = waveData.wave_pos_first;
	y[0] = 28000/**std::max_element(amp.begin(), amp.end())*/;
	y[1] = 30000/**std::max_element(amp.begin(), amp.end())*1.15*/;
	ui->widget->graph(1)->setData(x, y);

	x1[0] = waveData.wave_pos_second;
	x1[1] = waveData.wave_pos_second;
	y1[0] = 28000/**std::max_element(amp.begin(), amp.end())*/;
	y1[1] = 30000/**std::max_element(amp.begin(), amp.end()) *  1.15*/;
	ui->widget->graph(2)->setData(x1, y1);

	displayTxt += (QString::fromLocal8Bit("增益:"));
	displayTxt += (QString::number(waveData.curGain, 'f', 1));
	displayTxt += "db\n";
	displayTxt += QString::fromLocal8Bit("频率:");
	displayTxt += QString::number(waveData.excitation_freq, 'f', 1);
	displayTxt += "Mhz\n";
	if (PP_CONTROL_MODE == waveData.measureControlMode)
	{
		displayTxt += QString::fromLocal8Bit("固定PP");
	}
	else {
		displayTxt += QString::fromLocal8Bit("自动");
	}
	textLabel->setText(displayTxt);

	ui->widget->replot();
}

void form_param::ptn_clicked_send_param_slots()
{
    INT16 sendParam[2] = { 0 };
    sendParam[0] = ui->ldt_param_number->text().toInt();
    sendParam[1] = ui->ldt_param_value->text().toInt();
    emit sendParamChanged(sendParam[0], sendParam[1]);
}

void form_param::ptn_clicked_read_param_slots(){
    emit GetParam();
}

void Widget::update_para(const QByteArray &para_data){
    INT16 param_no = ui->ldt_param_number->text().toInt();
    INT16 param_value = para_data[param_no * 2] << 8 | para_data[param_no * 2 + 1];
    ui->ldt_param_value->setText(QString::number(param_value));
}
