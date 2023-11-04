#ifndef HYDROACOUSTICS_H
#define HYDROACOUSTICS_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

#pragma pack(push,1)


struct PUWVE //ответ от модема на включение командного режима
{
    unsigned char isPTMode = 0; //если 1, то пакетный режим включен
    unsigned char ptLocalAddress = 0; //адрес локального модема в пакетном режиме

};

struct PUWV0 // реакция устройства на поступивший от управляющей системы запрос
{
    unsigned char cmdID = 0; //Идентификатор обрабатываемой команды
    unsigned char errCode = 0; //Error code
    int counterAnswer2 = 0;// счетчик инкрементируется только если пришел запрос с кодом2
};

struct PUWV7
{
    float Pressure_mBar = 0;
    float Temperature_C = 0;
    float Depth_m = 0;
    float VCC_V = 0;
};

struct PUWV4
{
    unsigned char txChID = 0;
    unsigned char rcCmdID = 0;
    int counter = 0;
};

struct PUWV3
{
    unsigned char txChID = 0;
    unsigned char rcCmdID = 0;
    float propTime = 0;
    float MSR = 0;
    float Value = 0;
    int counter = 0;
};

struct uWave
{
    PUWVE puwve;
    PUWV0 puwv0;
    PUWV7 puwv7;
    PUWV4 puwv4;
    PUWV3 puwv3;
    int counterACKIdle = 0;
    QByteArray infoModem;
};

#pragma pack(pop)

class Hydroacoustics : public QObject
{
    Q_OBJECT
public:
    explicit Hydroacoustics( QObject *parent = nullptr);

    uWave uwave;
    void settings();
    void parseBuffer();
    void readData();
    void parsePUWV0(QByteArray msg);
    void parsePUWVE(QByteArray msg);
    void parsePUWV7(QByteArray msg);
    void parsePUWV4(QByteArray msg);
    void parsePUWV3(QByteArray msg);

protected:
    QSerialPort ha;
    QByteArray ha_buffer;
    QTimer timer;
    int crc (QByteArray msg);
    int crc_real(qint8 crc_in);
signals:
    void cmd1Received(); //
    void initCmd2Done();
    void initDone();
    void updateData(uWave uwave);
public slots:
    void sendCmd1();
    void sendCmd2();
    void sendCmd3();
    void sendCmd4();
    void sendCmd5();


};

#endif // HYDROACOUSTICS_H
