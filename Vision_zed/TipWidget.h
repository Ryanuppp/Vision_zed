#pragma once

#include <QWidget>
#include "ui_Tips.h"

class TipWidget : public QDialog{
    Q_OBJECT

public:
    TipWidget(QWidget *parent = Q_NULLPTR);
    ~TipWidget();

public slots:
  void setTips(int type);

private:
    Ui::Tips ui;
};
