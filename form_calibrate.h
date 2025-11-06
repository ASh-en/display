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
	std::vector<float>* pStepWorkActualThick;
	std::vector<float>* pStepWorkMeasureThick;

    void init_tblwidget(const std::vector<float>& actualData, const std::vector<float>& measureData);
};

#endif // FORM_CALIBRATE_H
