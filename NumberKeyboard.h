#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QMouseEvent>

class NumberKeyboard : public QDialog
{
    Q_OBJECT
public:
    explicit NumberKeyboard(QWidget *parent = nullptr);
    void setTarget(QLineEdit *edit);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

private slots:
    void onButtonClicked();

private:
    QLineEdit *m_target = nullptr;
    QPoint dragPos;
    QRect dragArea;  // 可拖动区域
};
