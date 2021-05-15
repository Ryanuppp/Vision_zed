#include "Vision_zed.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<std::string>("std::string");
    Vision_zed w;
    w.show();
    return a.exec();
}
