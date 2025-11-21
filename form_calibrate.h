#ifndef FORM_CALIBRATE_H
#define FORM_CALIBRATE_H

#include <QWidget>
#include "param_define.h"
#include "material_sound_speed_manager.h"

namespace Ui {
class form_calibrate;
}

class form_calibrate : public QWidget
{
    Q_OBJECT

public:
    explicit form_calibrate(QWidget *parent = nullptr);
    ~form_calibrate();

private:
    Ui::form_calibrate *ui;
	std::vector<float> pStepWorkActualThick{ 2.5, 5.0, 10.0, 20.0 };
	std::vector<float> pStepWorkMeasureThick{ 0.0, 0.0, 0.0, 0.0 };
    double current_thickness;

    void init_tblwidget(const std::vector<float>& actualData, const std::vector<float>& measureData);
    double calculateBestSpeed(const std::vector<double>& measuredThicknesses, const std::vector<double>& actualThicknesses, double init_speed);
    QMap<QString, QString> m_displayToOriginMaterial;
    void initMaterialInfo();
    MaterialSoundSpeedManager m_materialManager;
    void onMaterialChanged(const QString& displayName);
    
signals:
    void sendParamChanged(INT16 param_no, INT16 param_value);

public slots:
    void on_recv_thickness(const double& data);
private slots:
    void ptn_clicked_calculate();
    
    void on_btn_save_actual_data();
    void on_set_ultra_speed();


};

#endif // FORM_CALIBRATE_H
