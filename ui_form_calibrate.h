/********************************************************************************
** Form generated from reading UI file 'form_calibrate.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_CALIBRATE_H
#define UI_FORM_CALIBRATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_form_calibrate
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbl_thickness_2;
    QLineEdit *ldt_ultra_speed;
    QHBoxLayout *horizontalLayout;
    QLabel *lbl_thickness;
    QLineEdit *ldt_cur_thickness;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lbl_ultra_speed;
    QComboBox *cbx_material;
    QDoubleSpinBox *dsb_cal_speed;
    QTableWidget *tbl_step_workpiece;
    QVBoxLayout *verticalLayout;
    QPushButton *ptn_save_speed;
    QSpacerItem *verticalSpacer_2;
    QPushButton *ptn_calculate;
    QSpacerItem *verticalSpacer;
    QPushButton *ptn_set_speed;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *form_calibrate)
    {
        if (form_calibrate->objectName().isEmpty())
            form_calibrate->setObjectName(QString::fromUtf8("form_calibrate"));
        form_calibrate->resize(480, 182);
        widget = new QWidget(form_calibrate);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 471, 181));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lbl_thickness_2 = new QLabel(widget);
        lbl_thickness_2->setObjectName(QString::fromUtf8("lbl_thickness_2"));
        QFont font;
        font.setPointSize(16);
        lbl_thickness_2->setFont(font);

        horizontalLayout_2->addWidget(lbl_thickness_2);

        ldt_ultra_speed = new QLineEdit(widget);
        ldt_ultra_speed->setObjectName(QString::fromUtf8("ldt_ultra_speed"));
        ldt_ultra_speed->setFont(font);
        ldt_ultra_speed->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(ldt_ultra_speed);


        horizontalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lbl_thickness = new QLabel(widget);
        lbl_thickness->setObjectName(QString::fromUtf8("lbl_thickness"));
        lbl_thickness->setFont(font);

        horizontalLayout->addWidget(lbl_thickness);

        ldt_cur_thickness = new QLineEdit(widget);
        ldt_cur_thickness->setObjectName(QString::fromUtf8("ldt_cur_thickness"));
        ldt_cur_thickness->setFont(font);
        ldt_cur_thickness->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(ldt_cur_thickness);


        horizontalLayout_3->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lbl_ultra_speed = new QLabel(widget);
        lbl_ultra_speed->setObjectName(QString::fromUtf8("lbl_ultra_speed"));
        lbl_ultra_speed->setFont(font);

        horizontalLayout_4->addWidget(lbl_ultra_speed);

        cbx_material = new QComboBox(widget);
        cbx_material->addItem(QString());
        cbx_material->addItem(QString());
        cbx_material->addItem(QString());
        cbx_material->setObjectName(QString::fromUtf8("cbx_material"));
        cbx_material->setFont(font);

        horizontalLayout_4->addWidget(cbx_material);

        dsb_cal_speed = new QDoubleSpinBox(widget);
        dsb_cal_speed->setObjectName(QString::fromUtf8("dsb_cal_speed"));
        dsb_cal_speed->setFont(font);
        dsb_cal_speed->setDecimals(1);
        dsb_cal_speed->setMinimum(0.0);
        dsb_cal_speed->setMaximum(10000.0);
        dsb_cal_speed->setSingleStep(1.0);
        dsb_cal_speed->setValue(3090.0);
        dsb_cal_speed->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(dsb_cal_speed);


        verticalLayout_2->addLayout(horizontalLayout_4);

        tbl_step_workpiece = new QTableWidget(widget);
        tbl_step_workpiece->setObjectName(QString::fromUtf8("tbl_step_workpiece"));

        verticalLayout_2->addWidget(tbl_step_workpiece);


        horizontalLayout_5->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ptn_save_speed = new QPushButton(widget);
        ptn_save_speed->setObjectName(QString::fromUtf8("ptn_save_speed"));
        QFont font1;
        font1.setPointSize(14);
        ptn_save_speed->setFont(font1);

        verticalLayout->addWidget(ptn_save_speed);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        ptn_calculate = new QPushButton(widget);
        ptn_calculate->setObjectName(QString::fromUtf8("ptn_calculate"));
        ptn_calculate->setFont(font1);

        verticalLayout->addWidget(ptn_calculate);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        ptn_set_speed = new QPushButton(widget);
        ptn_set_speed->setObjectName(QString::fromUtf8("ptn_set_speed"));
        ptn_set_speed->setFont(font1);

        verticalLayout->addWidget(ptn_set_speed);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout_5->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_5);


        retranslateUi(form_calibrate);

        QMetaObject::connectSlotsByName(form_calibrate);
    } // setupUi

    void retranslateUi(QWidget *form_calibrate)
    {
        form_calibrate->setWindowTitle(QCoreApplication::translate("form_calibrate", "Form", nullptr));
        lbl_thickness_2->setText(QCoreApplication::translate("form_calibrate", "\345\275\223\345\211\215\345\243\260\351\200\237", nullptr));
        ldt_ultra_speed->setText(QCoreApplication::translate("form_calibrate", "3090.0", nullptr));
        lbl_thickness->setText(QCoreApplication::translate("form_calibrate", "\345\216\232\345\272\246     ", nullptr));
        ldt_cur_thickness->setText(QCoreApplication::translate("form_calibrate", "0.000", nullptr));
        lbl_ultra_speed->setText(QCoreApplication::translate("form_calibrate", "\350\256\241\347\256\227\345\243\260\351\200\237", nullptr));
        cbx_material->setItemText(0, QCoreApplication::translate("form_calibrate", "\351\223\235\345\220\210\351\207\2212219", nullptr));
        cbx_material->setItemText(1, QCoreApplication::translate("form_calibrate", "\351\223\235\345\220\210\351\207\2217050", nullptr));
        cbx_material->setItemText(2, QCoreApplication::translate("form_calibrate", "\351\223\235\345\220\210\351\207\2216061", nullptr));

        dsb_cal_speed->setValue(3090.0);
        ptn_save_speed->setText(QCoreApplication::translate("form_calibrate", "\344\277\235\345\255\230", nullptr));
        ptn_calculate->setText(QCoreApplication::translate("form_calibrate", "\350\256\241\347\256\227", nullptr));
        ptn_set_speed->setText(QCoreApplication::translate("form_calibrate", "\345\206\231\345\205\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class form_calibrate: public Ui_form_calibrate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_CALIBRATE_H
