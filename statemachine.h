#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QCoreApplication>
#include <QStateMachine>
#include <QState>
#include <QObject>
#include <QTimer>
#include "hydroacoustics.h"
#include "form.h"


enum class  State : qint8 {
    SheerState = 0,
    Navigation,
    EndState
};

static QMap<State,QString> statesMap = {
    {State::SheerState,"SheerState"},
    {State::Navigation, "Navigation"},
    {State::EndState, "EndState"}
};


class StateMachine : public QObject
{
    Q_OBJECT
public:
    StateMachine(QObject *parent = nullptr);
    Hydroacoustics *hydro = nullptr;

protected:

    QStateMachine machine;
    // Form *form = nullptr;
    QState *SheerState = nullptr, *Navigation = nullptr, *EndState = nullptr;
    QState *SendCmd1 = nullptr, *SendCmd2 = nullptr, *SendCmd3 = nullptr;
    QTimer timerCmd1;
    QTimer timerCmd2;
    QTimer timerSheerState;
    QTimer timerNavigation;
    QTimer timerDirect;
signals:
    void startNavigation();
    void finishNavigation();
    void cmd1Received();
    void cmd2Received();

    void SignStButtonRYB();
    void SignStButtonRBY();
    void SignStButtonBRY();
    void SignStButtonBYR();
    void SignStButtonYBR();
    void SignStButtonYRB();
    void SignStButtonHHH();

    void StSignGood();
    void StSignBed();
public slots:
    void initSheerStateSlot();
    void initNavigationSlot();
    void initEndState();
    void finishEndState();
    void finishSheerStateSlot();
    void finishNavigationSlot();
    void update(uWave uwave);

    void hudroStButtonRYB();
    void hudroStButtonRBY();
    void hudroStButtonBRY();
    void hudroStButtonBYR();
    void hudroStButtonYBR();
    void hudroStButtonYRB();
    void hudroStButtonHHH();

    void slotStBed();
    void slotStGood();

};

#endif // STATEMACHINE_H
