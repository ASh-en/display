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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_form_calibrate
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *lbl_ultra_speed;
    QComboBox *cbx_material;
    QLineEdit *ldt_ultra_speed;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbl_thickness;
    QLineEdit *ldt_cur_thickness;
    QSpacerItem *horizontalSpacer;
    QTableWidget *tbl_step_workpiece;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *ptn_save_speed;
    QPushButton *ptn_calculate;
    QSpacerItem *verticalSpacer_2;
    QPushButton *ptn_set_speed;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *form_calibrate)
    {
        if (form_calibrate->objectName().isEmpty())
            form_calibrate->setObjectName(QString::fromUtf8("form_calibrate"));
        form_calibrate->resize(470, 180);
        layoutWidget = new QWidget(form_calibrate);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 2, 451, 171));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lbl_ultra_speed = new QLabel(layoutWidget);
        lbl_ultra_speed->setObjectName(QString::fromUtf8("lbl_ultra_speed"));
        QFont font;
        font.setPointSize(16);
        lbl_ultra_speed->setFont(font);

        horizontalLayout->addWidget(lbl_ultra_speed);

        cbx_material = new QComboBox(layoutWidget);
        cbx_material->addItem(QString());
        cbx_material->addItem(QString());
        cbx_material->addItem(QString());
        cbx_material->setObjectName(QString::fromUtf8("cbx_material"));
        cbx_material->setFont(font);

        horizontalLayout->addWidget(cbx_material);

        ldt_ultra_speed = new QLineEdit(layoutWidget);
        ldt_ultra_speed->setObjectName(QString::fromUtf8("ldt_ultra_speed"));
        ldt_ultra_speed->setFont(font);
        ldt_ultra_speed->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(ldt_ultra_speed);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lbl_thickness = new QLabel(layoutWidget);
        lbl_thickness->setObjectName(QString::fromUtf8("lbl_thickness"));
        lbl_thickness->setFont(font);

        horizontalLayout_2->addWidget(lbl_thickness);

        ldt_cur_thickness = new QLineEdit(layoutWidget);
        ldt_cur_thickness->setObjectName(QString::fromUtf8("ldt_cur_thickness"));
        ldt_cur_thickness->setFont(font);
        ldt_cur_thickness->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(ldt_cur_thickness);

        horizontalSpacer = new QSpacerItem(185, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_2);

        tbl_step_workpiece = new QTableWidget(layoutWidget);
        tbl_step_workpiece->setObjectName(QString::fromUtf8("tbl_step_workpiece"));

        verticalLayout_2->addWidget(tbl_step_workpiece);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        ptn_save_speed = new QPushButton(layoutWidget);
        ptn_save_speed->setObjectName(QString::fromUtf8("ptn_save_speed"));
        QFont font1;
        font1.setPointSize(14);
        ptn_save_speed->setFont(font1);

        verticalLayout->addWidget(ptn_save_speed);

        ptn_calculate = new QPushButton(layoutWidget);
        ptn_calculate->setObjectName(QString::fromUtf8("ptn_calculate"));
        ptn_calculate->setFont(font1);

        verticalLayout->addWidget(ptn_calculate);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        ptn_set_speed = new QPushButton(layoutWidget);
        ptn_set_speed->setObjectName(QString::fromUtf8("ptn_set_speed"));
        ptn_set_speed->setFont(font1);

        verticalLayout->addWidget(ptn_set_speed);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_3);


        retranslateUi(form_calibrate);

        QMetaObject::connectSlotsByName(form_calibrate);
    } // setupUi

    void retranslateUi(QWidget *form_calibrate)
    {
        form_calibrate->setWindowTitle(QCoreApplication::translate("form_calibrate", "Form", nullptr));
        lbl_ultra_speed->setText(QCoreApplication::translate("form_calibrate", "\350\266\205\345\243\260\345\243\260\351\200\237", nullptr));
        cbx_material->setItemText(0, QCoreApplication::translate("form_calibrate", "\351\223\235\345\220\210\351\207\2212219", nullptr));
        cbx_material->setItemText(1, QCoreApplication::translate("form_calibrate", "\351\223\235\345\220\210\351\207\2217050", nullptr));
        cbx_material->setItemText(2, QCoreApplication::translate("form_calibrate", "\351\223\235\345\220\210\351\207\2216061", nullptr));

        ldt_ultra_speed->setText(QCoreApplication::translate("form_calibrate", "3090.0", nullptr));
        lbl_thickness->setText(QCoreApplication::translate("form_calibrate", "\345\216\232\345\272\246     ", nullptr));
        ldt_cur_thickness->setText(QCoreApplication::translate("form_calibrate", "0.000", nullptr));
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
