#include "TipWidget.h"

TipWidget::TipWidget(QWidget *parent)
    : QDialog(parent){
    ui.setupUi(this);
}

TipWidget::~TipWidget(){
}

void TipWidget::setTips(int type) {
  std::string tips;
  switch (type) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    break;
  default:
  }
  ui.tips->setText(QString::fromLocal8Bit(tips.c_str()));
}
