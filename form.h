#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private slots:
    void on_buttonRYB_clicked();

    void on_buttonRBY_clicked();

    void on_buttonBRY_clicked();

    void on_buttonBYR_clicked();

    void on_buttonYBR_clicked();

    void on_buttonYRB_clicked();

    void on_buttonHHH_clicked();
public slots:
    void indGood();
    void indBed();

signals:
    void buttonRYB();
    void buttonRBY();
    void buttonBRY();
    void buttonBYR();
    void buttonYBR();
    void buttonYRB();
    void buttonHHH();

private:
    Ui::Form *ui;
};

#endif // FORM_H
