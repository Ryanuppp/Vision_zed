/********************************************************************************
** Form generated from reading UI file 'TipWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIPWIDGET_H
#define UI_TIPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TipWidget
{
public:

    void setupUi(QWidget *TipWidget)
    {
        if (TipWidget->objectName().isEmpty())
            TipWidget->setObjectName(QString::fromUtf8("TipWidget"));
        TipWidget->resize(400, 300);

        retranslateUi(TipWidget);

        QMetaObject::connectSlotsByName(TipWidget);
    } // setupUi

    void retranslateUi(QWidget *TipWidget)
    {
        TipWidget->setWindowTitle(QApplication::translate("TipWidget", "TipWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TipWidget: public Ui_TipWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIPWIDGET_H
