/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *ip;
    QLabel *label_2;
    QLineEdit *port;
    QPushButton *btn_bind;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QListWidget *listWidget;
    QPushButton *btn_delete;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QTextEdit *textEdit;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(369, 321);
        gridLayout = new QGridLayout(Widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        ip = new QLineEdit(Widget);
        ip->setObjectName(QString::fromUtf8("ip"));

        gridLayout->addWidget(ip, 0, 1, 1, 1);

        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        port = new QLineEdit(Widget);
        port->setObjectName(QString::fromUtf8("port"));

        gridLayout->addWidget(port, 0, 3, 1, 1);

        btn_bind = new QPushButton(Widget);
        btn_bind->setObjectName(QString::fromUtf8("btn_bind"));

        gridLayout->addWidget(btn_bind, 0, 4, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        listWidget = new QListWidget(Widget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        btn_delete = new QPushButton(Widget);
        btn_delete->setObjectName(QString::fromUtf8("btn_delete"));

        verticalLayout->addWidget(btn_delete);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_2->addWidget(label_4);

        textEdit = new QTextEdit(Widget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout_2->addWidget(textEdit);


        gridLayout->addLayout(verticalLayout_2, 1, 3, 1, 2);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "IP", nullptr));
        ip->setText(QString());
        label_2->setText(QCoreApplication::translate("Widget", "Port", nullptr));
        port->setText(QCoreApplication::translate("Widget", "9000", nullptr));
        btn_bind->setText(QCoreApplication::translate("Widget", "\347\273\221\345\256\232", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\345\234\250\347\272\277\345\210\227\350\241\250", nullptr));
        btn_delete->setText(QCoreApplication::translate("Widget", "\345\210\240\351\231\244", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\346\266\210\346\201\257\345\210\227\350\241\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
