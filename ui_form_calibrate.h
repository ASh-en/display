/********************************************************************************
** Form generated from reading UI file 'form_calibrate.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_CALIBRATE_H
#define UI_FORM_CALIBRATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_form_calibrate
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *lbl_ultra_speed;
    QComboBox *comboBox;
    QLineEdit *ldt_ultra_speed;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *ptn_get_speed;
    QSpacerItem *verticalSpacer_2;
    QPushButton *ptn_set_speed;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *form_calibrate)
    {
        if (form_calibrate->objectName().isEmpty())
            form_calibrate->setObjectName(QStringLiteral("form_calibrate"));
        form_calibrate->resize(488, 182);
        layoutWidget = new QWidget(form_calibrate);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 461, 171));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lbl_ultra_speed = new QLabel(layoutWidget);
        lbl_ultra_speed->setObjectName(QStringLiteral("lbl_ultra_speed"));
        QFont font;
        font.setPointSize(16);
        lbl_ultra_speed->setFont(font);

        horizontalLayout->addWidget(lbl_ultra_speed);

        comboBox = new QComboBox(layoutWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setFont(font);

        horizontalLayout->addWidget(comboBox);

        ldt_ultra_speed = new QLineEdit(layoutWidget);
        ldt_ultra_speed->setObjectName(QStringLiteral("ldt_ultra_speed"));
        ldt_ultra_speed->setFont(font);
        ldt_ultra_speed->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(ldt_ultra_speed);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        listWidget = new QListWidget(layoutWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        horizontalLayout_2->addWidget(listWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        ptn_get_speed = new QPushButton(layoutWidget);
        ptn_get_speed->setObjectName(QStringLiteral("ptn_get_speed"));
        QFont font1;
        font1.setPointSize(14);
        ptn_get_speed->setFont(font1);

        verticalLayout->addWidget(ptn_get_speed);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        ptn_set_speed = new QPushButton(layoutWidget);
        ptn_set_speed->setObjectName(QStringLiteral("ptn_set_speed"));
        ptn_set_speed->setFont(font1);

        verticalLayout->addWidget(ptn_set_speed);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(form_calibrate);

        QMetaObject::connectSlotsByName(form_calibrate);
    } // setupUi

    void retranslateUi(QWidget *form_calibrate)
    {
        form_calibrate->setWindowTitle(QApplication::translate("form_calibrate", "Form", nullptr));
        lbl_ultra_speed->setText(QApplication::translate("form_calibrate", "\350\266\205\345\243\260\345\243\260\351\200\237", nullptr));
        comboBox->setItemText(0, QApplication::translate("form_calibrate", "\351\223\235\345\220\210\351\207\2212219", nullptr));
        comboBox->setItemText(1, QApplication::translate("form_calibrate", "\351\223\235\345\220\210\351\207\2217050", nullptr));
        comboBox->setItemText(2, QApplication::translate("form_calibrate", "\351\223\235\345\220\210\351\207\2216061", nullptr));

        ldt_ultra_speed->setText(QApplication::translate("form_calibrate", "3090.0", nullptr));
        ptn_get_speed->setText(QApplication::translate("form_calibrate", "\350\257\273\345\217\226", nullptr));
        ptn_set_speed->setText(QApplication::translate("form_calibrate", "\345\206\231\345\205\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class form_calibrate: public Ui_form_calibrate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_CALIBRATE_H
