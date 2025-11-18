#ifndef FORM_CALIBRATE_H
#define FORM_CALIBRATE_H

#include <QWidget>

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

    void init_tblwidget(const std::vector<float>& actualData, const std::vector<float>& measureData);
    double calculateBestSpeed(const std::vector<double>& measuredThicknesses, const std::vector<double>& actualThicknesses, double init_speed);
signals:
    void sendParamChanged(INT16 param_no, INT16 param_value);
};

#endif // FORM_CALIBRATE_H
