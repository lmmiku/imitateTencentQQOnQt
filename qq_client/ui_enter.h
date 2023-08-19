/********************************************************************************
** Form generated from reading UI file 'enter.ui'
**
** Created by: Qt User Interface Compiler version 6.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTER_H
#define UI_ENTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Enter
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout_2;
    QCheckBox *checkBox_Savepassword;
    QPushButton *btn_resiger;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_password;
    QLineEdit *lineEdit_account;
    QLabel *label_password;
    QLabel *label_account;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QCheckBox *checkBox_AIresiger;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_findPassword;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_newAccount;

    void setupUi(QWidget *Enter)
    {
        if (Enter->objectName().isEmpty())
            Enter->setObjectName(QString::fromUtf8("Enter"));
        Enter->resize(546, 475);
        Enter->setStyleSheet(QString::fromUtf8("#Enter{\n"
"border-image:url(:/res/backGround.jpg)\n"
"}"));
        verticalLayout = new QVBoxLayout(Enter);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget_2 = new QWidget(Enter);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(150, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        horizontalSpacer_5 = new QSpacerItem(150, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout->addWidget(widget_2);

        widget = new QWidget(Enter);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        checkBox_Savepassword = new QCheckBox(widget);
        checkBox_Savepassword->setObjectName(QString::fromUtf8("checkBox_Savepassword"));

        gridLayout_2->addWidget(checkBox_Savepassword, 1, 1, 1, 1);

        btn_resiger = new QPushButton(widget);
        btn_resiger->setObjectName(QString::fromUtf8("btn_resiger"));

        gridLayout_2->addWidget(btn_resiger, 3, 0, 1, 2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit_password = new QLineEdit(widget);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));

        gridLayout->addWidget(lineEdit_password, 1, 1, 1, 1);

        lineEdit_account = new QLineEdit(widget);
        lineEdit_account->setObjectName(QString::fromUtf8("lineEdit_account"));

        gridLayout->addWidget(lineEdit_account, 0, 1, 1, 1);

        label_password = new QLabel(widget);
        label_password->setObjectName(QString::fromUtf8("label_password"));

        gridLayout->addWidget(label_password, 1, 0, 1, 1);

        label_account = new QLabel(widget);
        label_account->setObjectName(QString::fromUtf8("label_account"));

        gridLayout->addWidget(label_account, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 2, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_2, 2, 1, 1, 1);

        checkBox_AIresiger = new QCheckBox(widget);
        checkBox_AIresiger->setObjectName(QString::fromUtf8("checkBox_AIresiger"));

        gridLayout_2->addWidget(checkBox_AIresiger, 1, 0, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addWidget(widget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_findPassword = new QPushButton(Enter);
        btn_findPassword->setObjectName(QString::fromUtf8("btn_findPassword"));

        horizontalLayout->addWidget(btn_findPassword);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_newAccount = new QPushButton(Enter);
        btn_newAccount->setObjectName(QString::fromUtf8("btn_newAccount"));

        horizontalLayout->addWidget(btn_newAccount);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Enter);

        QMetaObject::connectSlotsByName(Enter);
    } // setupUi

    void retranslateUi(QWidget *Enter)
    {
        Enter->setWindowTitle(QCoreApplication::translate("Enter", "Enter", nullptr));
        checkBox_Savepassword->setText(QCoreApplication::translate("Enter", "\344\277\235\345\255\230\345\257\206\347\240\201", nullptr));
        btn_resiger->setText(QCoreApplication::translate("Enter", "\347\231\273\345\275\225", nullptr));
        label_password->setText(QCoreApplication::translate("Enter", "\345\257\206\347\240\201:", nullptr));
        label_account->setText(QCoreApplication::translate("Enter", "\350\264\246\345\217\267:", nullptr));
        checkBox_AIresiger->setText(QCoreApplication::translate("Enter", "\350\207\252\345\212\250\347\231\273\345\275\225", nullptr));
        btn_findPassword->setText(QCoreApplication::translate("Enter", "\346\211\276\345\233\236\345\257\206\347\240\201", nullptr));
        btn_newAccount->setText(QCoreApplication::translate("Enter", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Enter: public Ui_Enter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTER_H
