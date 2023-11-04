#include "widget.h"
#include "hydroacoustics.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Hydroacoustics hydro;
    Widget w;
    QObject::connect(&w, &Widget::getInfo,&hydro,&Hydroacoustics::sendCmd1);
    QObject::connect(&w, &Widget::setCmdMode,&hydro,&Hydroacoustics::sendCmd2);
    QObject::connect(&w, &Widget::saveToFlash,&hydro,&Hydroacoustics::sendCmd3);
    QObject::connect(&w, &Widget::getACK,&hydro,&Hydroacoustics::sendCmd4);
    QObject::connect(&w,&Widget::getACK2,&hydro,&Hydroacoustics::sendCmd5);
    QObject::connect(&hydro, &Hydroacoustics::updateData, &w, &Widget::update);
    w.show();
    return a.exec();
}
