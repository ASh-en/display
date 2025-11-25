#ifndef FORM_MEASURE_H
#define FORM_MEASURE_H

#include <QWidget>
#include "dac8760.h"
#include "param_define.h"
#include <QMap>
#include "material_sound_speed_manager.h"

namespace Ui {
class form_measure;
}

class form_measure : public QWidget
{
    Q_OBJECT

public:
    explicit form_measure(QWidget *parent = nullptr);
    ~form_measure();
private:
    Ui::form_measure *ui;
    dac8760 _dac;
    int64_t recvDataNum;
    QMap<QString, QString> m_displayToOriginMaterial;
    void initMaterialInfo();
    MaterialSoundSpeedManager m_materialManager;

public slots:
    void onBatteryQuantityChanged(const int& data);
    void onRecvThhicknesData(const double& data);
    void onMaterialChanged(const QString& displayName);

signals:
    void sendStatusText(const QString& txt);
    /**参数号与参数值 */
    void sendParamChanged(INT16 param_no, INT16 param_val);
    


};

#endif // FORM_MEASURE_H

