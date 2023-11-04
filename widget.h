#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "hydroacoustics.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
signals:
    void getInfo();
    void setCmdMode();
    void saveToFlash();
    void getACK();
    void getACK2();
public slots:
    void update(uWave uwave);
};
#endif // WIDGET_H
