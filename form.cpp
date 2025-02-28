#include "form.h"
#include "ui_form.h"
#include <QDebug>

Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::on_buttonRYB_clicked()
{
    qDebug() << "all good";
    emit buttonRYB();
}


void Form::on_buttonRBY_clicked()
{
    emit buttonRBY();
}


void Form::on_buttonBRY_clicked()
{
    emit buttonBRY();
}


void Form::on_buttonBYR_clicked()
{
    emit buttonBYR();
}


void Form::on_buttonYBR_clicked()
{
    emit buttonYBR();
}


void Form::on_buttonYRB_clicked()
{
    emit buttonYRB();
}


void Form::on_buttonHHH_clicked()
{
    emit buttonHHH();
}


void Form::indGood()
{
    ui->labelIND->setText("GOOD");
}

void Form::indBed()
{
    ui->labelIND->setText("BED");
}

