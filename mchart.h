#ifndef MCHART_H
#define MCHART_H

#include <QObject>
#include <QtCharts>
#include <QSplineSeries>
#include <QChartView>
class mChart : public QObject
{
    Q_OBJECT
public:
    explicit mChart(QObject *parent = nullptr);

signals:

public slots:

public:
    void updateText(const QByteArray &para_data);
public:
     QLineSeries *lineSeries;
     QChart *myChart;
     QChartView *chartView;
     QScatterSeries *scatterSeries;
     QValueAxis *axisX;
     QValueAxis *axisY;

     QScatterSeries *scatterSeries2;
     QValueAxis *axisX2;
     QValueAxis *axisY2;
     QGraphicsSimpleTextItem *textItem;
};

#endif // MCHART_H
