/********************************************************************************
** Form generated from reading UI file 'Vision_zed.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISION_ZED_H
#define UI_VISION_ZED_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Vision_zedClass
{
public:
    QWidget *centralWidget;
    QLabel *left;
    QLabel *right;
    QLabel *label;
    QLabel *label_2;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QLabel *depth;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Vision_zedClass)
    {
        if (Vision_zedClass->objectName().isEmpty())
            Vision_zedClass->setObjectName(QString::fromUtf8("Vision_zedClass"));
        Vision_zedClass->resize(1500, 1000);
        centralWidget = new QWidget(Vision_zedClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        left = new QLabel(centralWidget);
        left->setObjectName(QString::fromUtf8("left"));
        left->setGeometry(QRect(60, 90, 640, 360));
        right = new QLabel(centralWidget);
        right->setObjectName(QString::fromUtf8("right"));
        right->setGeometry(QRect(760, 90, 640, 360));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 40, 72, 15));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(780, 40, 72, 15));
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(70, 60, 1341, 21));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(720, 70, 16, 851));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(50, 470, 1341, 21));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        depth = new QLabel(centralWidget);
        depth->setObjectName(QString::fromUtf8("depth"));
        depth->setGeometry(QRect(50, 520, 640, 360));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(780, 550, 181, 41));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(780, 640, 181, 41));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(780, 720, 181, 41));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(1000, 540, 181, 41));
        Vision_zedClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Vision_zedClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1500, 26));
        Vision_zedClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Vision_zedClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Vision_zedClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Vision_zedClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Vision_zedClass->setStatusBar(statusBar);

        retranslateUi(Vision_zedClass);
        QObject::connect(pushButton, SIGNAL(clicked()), Vision_zedClass, SLOT(onBtn_openCamera()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), Vision_zedClass, SLOT(onBtn_closeCamera()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), Vision_zedClass, SLOT(onBtn_record()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), Vision_zedClass, SLOT(onBtn_startProcess()));

        QMetaObject::connectSlotsByName(Vision_zedClass);
    } // setupUi

    void retranslateUi(QMainWindow *Vision_zedClass)
    {
        Vision_zedClass->setWindowTitle(QApplication::translate("Vision_zedClass", "Vision_zed", nullptr));
        left->setText(QString());
        right->setText(QString());
        label->setText(QApplication::translate("Vision_zedClass", "LEFT", nullptr));
        label_2->setText(QApplication::translate("Vision_zedClass", "right", nullptr));
        depth->setText(QString());
        pushButton->setText(QApplication::translate("Vision_zedClass", "openCamera", nullptr));
        pushButton_2->setText(QApplication::translate("Vision_zedClass", "CloseCamera", nullptr));
        pushButton_3->setText(QApplication::translate("Vision_zedClass", "Record/Stop", nullptr));
        pushButton_4->setText(QApplication::translate("Vision_zedClass", "startProcess", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Vision_zedClass: public Ui_Vision_zedClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISION_ZED_H
