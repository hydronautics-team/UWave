#include <QApplication>
#include "statemachine.h"
#include <QObject>
#include "form.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StateMachine st;



    Form form;

    // QObject::connect(&form, &Form::buttonBRY, &st, &StateMachine.hydro::hudroButtonBRY);
    QObject::connect(&form, &Form::buttonBRY, &st, &StateMachine::hudroStButtonBRY);
    QObject::connect(&form, &Form::buttonBYR, &st, &StateMachine::hudroStButtonBYR);
    QObject::connect(&form, &Form::buttonRYB, &st, &StateMachine::hudroStButtonRYB);
    QObject::connect(&form, &Form::buttonRBY, &st, &StateMachine::hudroStButtonRBY);
    QObject::connect(&form, &Form::buttonYRB, &st, &StateMachine::hudroStButtonYRB);
    QObject::connect(&form, &Form::buttonYBR, &st, &StateMachine::hudroStButtonYBR);
    QObject::connect(&form, &Form::buttonHHH, &st, &StateMachine::hudroStButtonHHH);

    QObject::connect(&st, &StateMachine::StSignBed, &form, &Form::indBed);
    QObject::connect(&st, &StateMachine::StSignGood, &form, &Form::indGood);
    form.show();

    return a.exec();
}
