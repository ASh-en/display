/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *lbl_connect_status;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_poweroff;
    QPushButton *btn_reboot;
    QPushButton *btn_connect;
    QStackedWidget *stackedWidget;
    QWidget *pageMeasure;
    QWidget *pageCalibrate;
    QWidget *pageParam;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnMeasure;
    QPushButton *btnCalibrate;
    QPushButton *btnParam;
    QLabel *lbl_status;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(480, 320);
        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(1, 2, 471, 311));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(true);
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        label->setLayoutDirection(Qt::RightToLeft);
        label->setTextFormat(Qt::PlainText);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label);

        lbl_connect_status = new QLabel(layoutWidget);
        lbl_connect_status->setObjectName(QString::fromUtf8("lbl_connect_status"));
        lbl_connect_status->setFont(font);
        lbl_connect_status->setLayoutDirection(Qt::RightToLeft);
        lbl_connect_status->setTextFormat(Qt::PlainText);
        lbl_connect_status->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lbl_connect_status);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btn_poweroff = new QPushButton(layoutWidget);
        btn_poweroff->setObjectName(QString::fromUtf8("btn_poweroff"));
        btn_poweroff->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{font-size:14px; padding:6px 25px; background:#2E86AB; color:white; border:none; border-radius:3px;}\n"
"QPushButton:pressed{\n"
"    background:#0F5C7D; \n"
"}"));
        btn_poweroff->setCheckable(true);
        btn_poweroff->setChecked(true);

        horizontalLayout_3->addWidget(btn_poweroff);

        btn_reboot = new QPushButton(layoutWidget);
        btn_reboot->setObjectName(QString::fromUtf8("btn_reboot"));
        btn_reboot->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{font-size:14px; padding:6px 25px; background:#2E86AB; color:white; border:none; border-radius:3px;}\n"
"QPushButton:hover{background:#3A9FCF;}\n"
"QPushButton:pressed{background:#1A5F7A; border:2px solid #4FB3D9;}\n"
"       "));
        btn_reboot->setCheckable(true);
        btn_reboot->setChecked(true);

        horizontalLayout_3->addWidget(btn_reboot);

        btn_connect = new QPushButton(layoutWidget);
        btn_connect->setObjectName(QString::fromUtf8("btn_connect"));
        btn_connect->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{font-size:14px; padding:6px 25px; background:#2E86AB; color:white; border:none; border-radius:3px;}\n"
"QPushButton:hover{background:#3A9FCF;}\n"
"QPushButton:pressed{background:#1A5F7A; border:2px solid #4FB3D9;}\n"
"       "));
        btn_connect->setCheckable(true);
        btn_connect->setChecked(true);

        horizontalLayout_3->addWidget(btn_connect);


        verticalLayout->addLayout(horizontalLayout_3);

        stackedWidget = new QStackedWidget(layoutWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        pageMeasure = new QWidget();
        pageMeasure->setObjectName(QString::fromUtf8("pageMeasure"));
        stackedWidget->addWidget(pageMeasure);
        pageCalibrate = new QWidget();
        pageCalibrate->setObjectName(QString::fromUtf8("pageCalibrate"));
        stackedWidget->addWidget(pageCalibrate);
        pageParam = new QWidget();
        pageParam->setObjectName(QString::fromUtf8("pageParam"));
        stackedWidget->addWidget(pageParam);

        verticalLayout->addWidget(stackedWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(15);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnMeasure = new QPushButton(layoutWidget);
        btnMeasure->setObjectName(QString::fromUtf8("btnMeasure"));
        btnMeasure->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{font-size:14px; padding:6px 25px; background:#2E86AB; color:white; border:none; border-radius:3px;}\n"
"QPushButton:hover{background:#3A9FCF;}\n"
"QPushButton:checked{background:#1A5F7A; border:2px solid #4FB3D9;}\n"
"       "));
        btnMeasure->setCheckable(true);
        btnMeasure->setChecked(true);

        horizontalLayout->addWidget(btnMeasure);

        btnCalibrate = new QPushButton(layoutWidget);
        btnCalibrate->setObjectName(QString::fromUtf8("btnCalibrate"));
        btnCalibrate->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{font-size:14px; padding:6px 25px; background:#2E86AB; color:white; border:none; border-radius:3px;}\n"
"QPushButton:hover{background:#3A9FCF;}\n"
"QPushButton:checked{background:#1A5F7A; border:2px solid #4FB3D9;}\n"
"       "));
        btnCalibrate->setCheckable(true);

        horizontalLayout->addWidget(btnCalibrate);

        btnParam = new QPushButton(layoutWidget);
        btnParam->setObjectName(QString::fromUtf8("btnParam"));
        btnParam->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{font-size:14px; padding:6px 25px; background:#2E86AB; color:white; border:none; border-radius:3px;}\n"
"QPushButton:hover{background:#3A9FCF;}\n"
"QPushButton:checked{background:#1A5F7A; border:2px solid #4FB3D9;}\n"
"       "));
        btnParam->setCheckable(true);

        horizontalLayout->addWidget(btnParam);


        verticalLayout->addLayout(horizontalLayout);

        lbl_status = new QLabel(layoutWidget);
        lbl_status->setObjectName(QString::fromUtf8("lbl_status"));

        verticalLayout->addWidget(lbl_status);


        retranslateUi(Widget);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "\347\224\265\347\243\201\350\266\205\345\243\260\346\265\213\345\216\232\347\263\273\347\273\237", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\350\277\236\346\216\245\347\212\266\346\200\201", nullptr));
        lbl_connect_status->setText(QCoreApplication::translate("Widget", "\346\234\252\350\277\236\346\216\245", nullptr));
        btn_poweroff->setText(QCoreApplication::translate("Widget", "\345\205\263\346\234\272", nullptr));
        btn_reboot->setText(QCoreApplication::translate("Widget", "\351\207\215\345\220\257", nullptr));
        btn_connect->setText(QCoreApplication::translate("Widget", "\350\277\236\346\216\245", nullptr));
        btnMeasure->setText(QCoreApplication::translate("Widget", "\346\265\213\351\207\217", nullptr));
        btnCalibrate->setText(QCoreApplication::translate("Widget", "\346\240\241\345\207\206", nullptr));
        btnParam->setText(QCoreApplication::translate("Widget", "\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        lbl_status->setText(QCoreApplication::translate("Widget", "\347\212\266\346\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
