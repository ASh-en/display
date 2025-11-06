#ifndef FORM_PARAM_H
#define FORM_PARAM_H
#include <QTimer>
#include <QWidget>
#include "qcustomplot.h" 
#include "type_define.h"
#include "param_define.h"
#include <QVector>
#include <QString

namespace Ui {
class form_param;
}

class form_param : public QWidget
{
    Q_OBJECT

public:
    explicit form_param(QWidget *parent = nullptr);
    ~form_param();

private:
    Ui::form_param *ui;
    void initPlotGraph();
    QTimer* pTimer;
    QCPItemText* textLabel;
    void updatePlotGraph(const WAVE_DATA& waveData);
    void set_signal_connect_slot();
   

public slots:
   void onRecvWaveData(const WAVE_DATA& cur_data);
   void update_para(const QByteArray &para_data);
private slots:
   void ptn_clicked_send_param_slots();
   void ptn_clicked_read_param_slots();
signals:
    void sendParamChanged(INT16 param_no, INT16 param_val);
    void GetParam();
};

#endif // FORM_PARAM_H.
