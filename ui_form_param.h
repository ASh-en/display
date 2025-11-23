/********************************************************************************
** Form generated from reading UI file 'form_param.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_PARAM_H
#define UI_FORM_PARAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_form_param
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QCustomPlot *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *ldt_param_number;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *ldt_param_value;
    QPushButton *ptn_get_param;
    QPushButton *ptn_set_param;

    void setupUi(QWidget *form_param)
    {
        if (form_param->objectName().isEmpty())
            form_param->setObjectName(QString::fromUtf8("form_param"));
        form_param->resize(470, 182);
        layoutWidget = new QWidget(form_param);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 10, 461, 161));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        widget = new QCustomPlot(layoutWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(330, 0));

        horizontalLayout_3->addWidget(widget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        ldt_param_number = new QLineEdit(layoutWidget);
        ldt_param_number->setObjectName(QString::fromUtf8("ldt_param_number"));
        ldt_param_number->setFont(font);

        horizontalLayout->addWidget(ldt_param_number);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        ldt_param_value = new QLineEdit(layoutWidget);
        ldt_param_value->setObjectName(QString::fromUtf8("ldt_param_value"));
        ldt_param_value->setFont(font);

        horizontalLayout_2->addWidget(ldt_param_value);


        verticalLayout->addLayout(horizontalLayout_2);

        ptn_get_param = new QPushButton(layoutWidget);
        ptn_get_param->setObjectName(QString::fromUtf8("ptn_get_param"));
        ptn_get_param->setFont(font);

        verticalLayout->addWidget(ptn_get_param);

        ptn_set_param = new QPushButton(layoutWidget);
        ptn_set_param->setObjectName(QString::fromUtf8("ptn_set_param"));
        ptn_set_param->setFont(font);

        verticalLayout->addWidget(ptn_set_param);


        horizontalLayout_3->addLayout(verticalLayout);


        retranslateUi(form_param);

        QMetaObject::connectSlotsByName(form_param);
    } // setupUi

    void retranslateUi(QWidget *form_param)
    {
        form_param->setWindowTitle(QCoreApplication::translate("form_param", "Form", nullptr));
        label->setText(QCoreApplication::translate("form_param", "\345\217\202\346\225\260\345\217\267", nullptr));
        label_2->setText(QCoreApplication::translate("form_param", "\345\217\202\346\225\260\345\200\274", nullptr));
        ptn_get_param->setText(QCoreApplication::translate("form_param", "\350\257\273\345\217\226", nullptr));
        ptn_set_param->setText(QCoreApplication::translate("form_param", "\345\206\231\345\205\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class form_param: public Ui_form_param {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_PARAM_H
