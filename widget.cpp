#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->btnInfo,&QPushButton::clicked,this,&Widget::getInfo);
    connect(ui->btnSaveToFlash,&QPushButton::clicked,this,&Widget::saveToFlash);
    connect(ui->btnSetCmdMode,&QPushButton::clicked,this,&Widget::setCmdMode);
    connect(ui->btnACK,&QPushButton::clicked,this,&Widget::getACK);
    connect(ui->btnACK_2,&QPushButton::clicked,this,&Widget::getACK2);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::update(uWave uwave)
{
    ui->cmdIDAn->setNum(uwave.puwv0.cmdID);
    ui->errCodeAn->setNum(uwave.puwv0.errCode);
    ui->Pressure_mBarAn->setNum(uwave.puwv7.Pressure_mBar);
    ui->Temperature_CAn->setNum(uwave.puwv7.Temperature_C);
    ui->Depth_mAn->setNum(uwave.puwv7.Depth_m);
    ui->VCC_VAn->setNum(uwave.puwv7.VCC_V);
    ui->txChIDAn->setNum(uwave.puwv4.txChID);
    ui->rcCmdIDAn->setNum(uwave.puwv4.rcCmdID);
    ui->counterAn->setNum(uwave.puwv4.counter);
    ui->txChIDAn_2->setNum(uwave.puwv3.txChID);
    ui->rcCmdIDAn_2->setNum(uwave.puwv3.rcCmdID);
    ui->propTimeAn->setNum(uwave.puwv3.propTime);
    ui->MSRAn->setNum(uwave.puwv3.MSR);
    ui->ValueAn->setNum(uwave.puwv3.Value);
    ui->counterAn_2->setNum(uwave.puwv3.counter);
    ui->textBrowser->append(uwave.infoModem);
    ui->counterAnswerAn->setNum(uwave.puwv0.counterAnswer2);
}

