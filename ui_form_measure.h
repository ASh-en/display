/********************************************************************************
** Form generated from reading UI file 'form_measure.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_MEASURE_H
#define UI_FORM_MEASURE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_form_measure
{
public:
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *lbl_electric_quantity;
    QProgressBar *progressBar_elec_quantity;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbl_thickness;
    QLineEdit *ldt_thickness;
    QLabel *lbl_thickness_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lbl_output;
    QLineEdit *ldt_output;
    QLabel *lbl_output_2;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QComboBox *cbx_material;
    QLabel *lbl_ultra_speed;
    QLineEdit *ldt_ultra_speed;

    void setupUi(QWidget *form_measure)
    {
        if (form_measure->objectName().isEmpty())
            form_measure->setObjectName(QStringLiteral("form_measure"));
        form_measure->resize(480, 240);
        groupBox = new QGroupBox(form_measure);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 311, 171));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 291, 143));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lbl_electric_quantity = new QLabel(layoutWidget);
        lbl_electric_quantity->setObjectName(QStringLiteral("lbl_electric_quantity"));
        QFont font;
        font.setPointSize(16);
        lbl_electric_quantity->setFont(font);

        horizontalLayout->addWidget(lbl_electric_quantity);

        progressBar_elec_quantity = new QProgressBar(layoutWidget);
        progressBar_elec_quantity->setObjectName(QStringLiteral("progressBar_elec_quantity"));
        QFont font1;
        font1.setPointSize(20);
        progressBar_elec_quantity->setFont(font1);
        progressBar_elec_quantity->setValue(24);

        horizontalLayout->addWidget(progressBar_elec_quantity);

        horizontalSpacer = new QSpacerItem(42, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lbl_thickness = new QLabel(layoutWidget);
        lbl_thickness->setObjectName(QStringLiteral("lbl_thickness"));
        lbl_thickness->setFont(font);
        lbl_thickness->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lbl_thickness);

        ldt_thickness = new QLineEdit(layoutWidget);
        ldt_thickness->setObjectName(QStringLiteral("ldt_thickness"));
        ldt_thickness->setFont(font1);
        ldt_thickness->setCursorPosition(4);
        ldt_thickness->setAlignment(Qt::AlignCenter);
        ldt_thickness->setReadOnly(true);

        horizontalLayout_2->addWidget(ldt_thickness);

        lbl_thickness_2 = new QLabel(layoutWidget);
        lbl_thickness_2->setObjectName(QStringLiteral("lbl_thickness_2"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbl_thickness_2->sizePolicy().hasHeightForWidth());
        lbl_thickness_2->setSizePolicy(sizePolicy);
        lbl_thickness_2->setMinimumSize(QSize(42, 0));
        lbl_thickness_2->setFont(font);
        lbl_thickness_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lbl_thickness_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lbl_output = new QLabel(layoutWidget);
        lbl_output->setObjectName(QStringLiteral("lbl_output"));
        lbl_output->setFont(font);
        lbl_output->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(lbl_output);

        ldt_output = new QLineEdit(layoutWidget);
        ldt_output->setObjectName(QStringLiteral("ldt_output"));
        ldt_output->setFont(font1);
        ldt_output->setCursorPosition(4);
        ldt_output->setAlignment(Qt::AlignCenter);
        ldt_output->setReadOnly(true);

        horizontalLayout_3->addWidget(ldt_output);

        lbl_output_2 = new QLabel(layoutWidget);
        lbl_output_2->setObjectName(QStringLiteral("lbl_output_2"));
        sizePolicy.setHeightForWidth(lbl_output_2->sizePolicy().hasHeightForWidth());
        lbl_output_2->setSizePolicy(sizePolicy);
        lbl_output_2->setMinimumSize(QSize(42, 0));
        lbl_output_2->setFont(font);
        lbl_output_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(lbl_output_2);


        verticalLayout->addLayout(horizontalLayout_3);

        groupBox_2 = new QGroupBox(form_measure);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(310, -1, 161, 171));
        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 20, 151, 143));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        cbx_material = new QComboBox(layoutWidget1);
        cbx_material->addItem(QString());
        cbx_material->addItem(QString());
        cbx_material->addItem(QString());
        cbx_material->setObjectName(QStringLiteral("cbx_material"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cbx_material->sizePolicy().hasHeightForWidth());
        cbx_material->setSizePolicy(sizePolicy1);
        cbx_material->setMinimumSize(QSize(0, 38));
        cbx_material->setFont(font);

        verticalLayout_2->addWidget(cbx_material);

        lbl_ultra_speed = new QLabel(layoutWidget1);
        lbl_ultra_speed->setObjectName(QStringLiteral("lbl_ultra_speed"));
        sizePolicy1.setHeightForWidth(lbl_ultra_speed->sizePolicy().hasHeightForWidth());
        lbl_ultra_speed->setSizePolicy(sizePolicy1);
        lbl_ultra_speed->setMinimumSize(QSize(0, 19));
        lbl_ultra_speed->setFont(font);
        lbl_ultra_speed->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lbl_ultra_speed);

        ldt_ultra_speed = new QLineEdit(layoutWidget1);
        ldt_ultra_speed->setObjectName(QStringLiteral("ldt_ultra_speed"));
        ldt_ultra_speed->setMinimumSize(QSize(0, 40));
        ldt_ultra_speed->setFont(font);
        ldt_ultra_speed->setCursorPosition(6);
        ldt_ultra_speed->setAlignment(Qt::AlignCenter);
        ldt_ultra_speed->setReadOnly(true);

        verticalLayout_2->addWidget(ldt_ultra_speed);

        groupBox_2->raise();
        groupBox->raise();

        retranslateUi(form_measure);

        QMetaObject::connectSlotsByName(form_measure);
    } // setupUi

    void retranslateUi(QWidget *form_measure)
    {
        form_measure->setWindowTitle(QApplication::translate("form_measure", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("form_measure", "\345\256\236\346\227\266\346\230\276\347\244\272", nullptr));
        lbl_electric_quantity->setText(QApplication::translate("form_measure", "\345\211\251\344\275\231\347\224\265\351\207\217", nullptr));
        lbl_thickness->setText(QApplication::translate("form_measure", "\345\275\223\345\211\215\345\216\232\345\272\246", nullptr));
        ldt_thickness->setText(QApplication::translate("form_measure", "0.00", nullptr));
        lbl_thickness_2->setText(QApplication::translate("form_measure", "mm", nullptr));
        lbl_output->setText(QApplication::translate("form_measure", "\347\224\265\346\265\201\350\276\223\345\207\272", nullptr));
        ldt_output->setText(QApplication::translate("form_measure", "4.00", nullptr));
        lbl_output_2->setText(QApplication::translate("form_measure", "mA", nullptr));
        groupBox_2->setTitle(QApplication::translate("form_measure", "\346\235\220\346\226\231\351\200\211\346\213\251", nullptr));
        cbx_material->setItemText(0, QApplication::translate("form_measure", "\351\223\235\345\220\210\351\207\2212219", nullptr));
        cbx_material->setItemText(1, QApplication::translate("form_measure", "\351\223\235\345\220\210\351\207\2217050", nullptr));
        cbx_material->setItemText(2, QApplication::translate("form_measure", "\351\223\235\345\220\210\351\207\2216061", nullptr));

        lbl_ultra_speed->setText(QApplication::translate("form_measure", "\350\266\205\345\243\260\345\243\260\351\200\237", nullptr));
        ldt_ultra_speed->setText(QApplication::translate("form_measure", "3090.0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class form_measure: public Ui_form_measure {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_MEASURE_H
