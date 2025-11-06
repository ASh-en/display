#include "mchart.h"

mChart::mChart(QObject *parent) : QObject(parent)
{
    myChart = new QChart();
    lineSeries = new QLineSeries();
    chartView = new QChartView(myChart);
    for(quint16 i=0; i<1500; i++)
    {
        lineSeries->append(i,0);
    }
    qreal minX = 5.0;
    qreal maxX = 20.0;
    for (int i = 0; i < lineSeries->count(); ++i) {
        QPointF point = lineSeries->at(i);
        qreal newX = minX + ((maxX - minX) / 1500.0) * point.x();
        lineSeries->replace(i, newX, point.y());
    }

    lineSeries->setName("系列2");
    myChart->addSeries(lineSeries);
 //   myChart->createDefaultAxes();
    myChart->setTheme(QChart::ChartThemeDark);
//    myChart->setTitle("wave");
    myChart->legend()->hide();

    axisX = new QValueAxis;
    axisX->setLabelFormat("%i us");
    axisX->setRange(minX, maxX); // 设置 X 轴范围
    myChart->addAxis(axisX, Qt::AlignBottom); // 将 X 轴添加到图表底部
    lineSeries->attachAxis(axisX);


    axisY = new QValueAxis;
    axisY->setLabelFormat("%i"); // 设置 Y 轴标签格式
    axisY->setRange(-30000, 30000); // 设置 Y 轴范围
    myChart->addAxis(axisY, Qt::AlignLeft); // 将 Y 轴添加到图表左侧
    lineSeries->attachAxis(axisY);

    chartView->setChart(myChart);
    chartView->resize(QSize(100,300));
    chartView->setRenderHints(QPainter::Antialiasing);

    // 创建文本项
    textItem = new QGraphicsSimpleTextItem("增益:30db  频率:5.0MH  固定PP");
    textItem->setFont(QFont("Arial", 10)); // 设置字体和大小
    QGraphicsScene *scene = chartView->scene();
    QRectF chartRect = myChart->plotArea(); // 获取绘图区域
    QPointF topRight = chartRect.topRight();
    topRight.setX(topRight.x() + 200); // 从右侧边缘向左移动 20 个像素
    topRight.setY(topRight.y() + 20); // 从顶部边缘向下移动 20 个像素
    textItem->setPos(topRight);
    scene->addItem(textItem);


    QColor backgroundColor(Qt::GlobalColor::white);
    myChart->setBackgroundBrush(QBrush(backgroundColor));

    QColor axisColor(Qt::GlobalColor::black);
    axisX->setLabelsColor(axisColor);
    axisX->setLinePenColor(axisColor);
    axisY->setLabelsColor(axisColor);
    axisY->setLinePenColor(axisColor);

    QGraphicsSimpleTextItem *xAxisLabel = new QGraphicsSimpleTextItem("时间");
     xAxisLabel->setFont(QFont("Arial", 12));
     QPointF xAxisPos = QPointF(235, 230);
     xAxisLabel->setPos(xAxisPos);
     scene->addItem(xAxisLabel);

     QGraphicsSimpleTextItem *yAxisLabel = new QGraphicsSimpleTextItem("增益");
         yAxisLabel->setFont(QFont("Arial", 12));
         QPointF yAxisPos = QPointF(25 ,108);
         yAxisLabel->setPos(yAxisPos);
         scene->addItem(yAxisLabel);

         // 创建一个新的 QScatterSeries 用于标记点
         scatterSeries = new QScatterSeries();
         scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
         scatterSeries->setMarkerSize(10); // 设置标记点的大小
         scatterSeries->setColor(Qt::blue); // 设置标记点的颜色

         myChart->addSeries(scatterSeries);
         scatterSeries->attachAxis(axisX);
         scatterSeries->attachAxis(axisY);

         scatterSeries2 = new QScatterSeries();
         scatterSeries2->setMarkerShape(QScatterSeries::MarkerShapeCircle);
         scatterSeries2->setMarkerSize(10); // 设置标记点的大小
         scatterSeries2->setColor(Qt::red); // 设置标记点的颜色

         myChart->addSeries(scatterSeries2);
         scatterSeries2->attachAxis(axisX);
         scatterSeries2->attachAxis(axisY);

}
void mChart::updateText(const QByteArray &para_data) {
    // 使用 arg() 构建字符串
    QString text = QString("增益:%1db  频率:%2MH  固定PP").arg(((static_cast<quint16>(para_data[26])<<8)|static_cast<quint8>(para_data[27]))/10.0).arg(((static_cast<quint16>(para_data[48])<<8)|static_cast<quint8>(para_data[49]))/100.0);

    // 更新文本内容
    textItem->setText(text);
}