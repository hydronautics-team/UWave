#include "hydroacoustics.h"
#include <QDebug>

Hydroacoustics::Hydroacoustics(QObject *parent) : QObject(parent)
{
    ha.setBaudRate(9600);
    ha.setPortName("COM19");
    ha.setDataBits(QSerialPort::Data8);
    ha.setStopBits(QSerialPort::OneStop);
    ha.setParity(QSerialPort::NoParity);
    ha.setFlowControl(QSerialPort::NoFlowControl);

    if (ha.open(QIODevice::ReadWrite)){
        qDebug()<<" port was opened";
        settings();
    }
    else {
        qDebug()<<" error open port "<< ha.errorString();
    }
    connect(&ha, &QSerialPort::readyRead, this, [this](){

        qDebug()<<ha_buffer.append(ha.readAll());
        qDebug() << "ha_buffer " << ha_buffer;
        int size = ha_buffer.size();
        char end = ha_buffer.at(size - 1);
        if (end == 10)
        {
            uwave.infoModem = ha_buffer;
            parseBuffer();
        }
    });
    timer.start(5000);


}

void Hydroacoustics::settings()
{
    // char PUWV6[24] = "$PUWV6,1,1,1,1,1,1*32\r\n"; // включаем передачу сообщений о состоянии модема
    char PUWV6[18] = "$PUWVF,1,1,0*5E\r\n"; // включаем передачу сообщений о состоянии модема
    // $PUWVF,1,1,0*5E
    ha.write(PUWV6, 18);
    qDebug() << "отправил первую команду и жду";
    ha.waitForBytesWritten();
    qDebug() << "все отправилось, жду ответа";
   // ha.waitForReadyRead();
    qDebug() << "что-то прислал иду смотреть";
    readData();
}

void Hydroacoustics::readData()
{

    qDebug() << "ha_buffer " << ha_buffer;
    parseBuffer();
}

int Hydroacoustics::crc(QByteArray tmp)
{
    int crc_ = 0;
    for (int i = 0; i <= tmp.size(); i++)
        crc_^=tmp[i];
    return crc_;
}

void Hydroacoustics::sendCmd1()
{
    settings();
    // char PUWVInvfo[14]="$PUWV?,0*27\r\n";
    // qDebug()<<"bytes written :" << ha.write(PUWVInvfo, 14);
    // ha.waitForBytesWritten();
}

void Hydroacoustics::sendCmd2()
{
//    char PUWV1[30] = "$PUWV1,0,0,0.,1,1,9.8067*35\r\n"; // включаем передачу сообщений о состоянии модема
//    char PUWV1[30] = "$PUWV1,1,0,0.,1,1,9.8067*34\r\n";
    char PUWV1[30] = "$PUWV1,0,1,0.,1,0,9.8067*01\r\n";
    qDebug()<<"bytes written :" << ha.write(PUWV1, 30);
    ha.waitForBytesWritten();
}

void Hydroacoustics::sendCmd3()
{
    char PUWV6[24] = "$PUWV6,1,1,1,1,1,1*32\r\n"; // включаем передачу сообщений о состоянии модема
    qDebug()<<"bytes written :" << ha.write(PUWV6, 24);
    ha.waitForBytesWritten();
}

void Hydroacoustics::sendCmd4()
{
    char PUWV2[18] = "$PUWV2,0,0,2*28\r\n"; // запрос
//    char PUWV2[18] = "$PUWV2,1,1,2*28\r\n";
//    char PUWV2[18] = "$PUWV2,1,0,2*29\r\n"; //передача 1 прием 0
    qDebug()<<"bytes written :" << ha.write(PUWV2, 18);
    ha.waitForBytesWritten();
}

void Hydroacoustics::sendCmd5()
{
    char PUWV2[18] = "$PUWV2,1,0,2*29\r\n"; //передача 1 прием 0
    qDebug()<<"bytes written :" << ha.write(PUWV2, 18);
    ha.waitForBytesWritten();
}

int Hydroacoustics::crc_real(qint8 crc_in)
{
    int crc_real = 0;
    if ((int (ha_buffer[crc_in+1]))>57)
    crc_real += ((int (ha_buffer[crc_in+1]))-'0'-7)*16;
    else crc_real += ((int (ha_buffer[crc_in+1]))-'0')*16;
    if ((int (ha_buffer[crc_in+2]))>57)
    crc_real += ((int (ha_buffer[crc_in+2]))-'0') -7;
    else crc_real += ((int (ha_buffer[crc_in+2]))-'0');
    return crc_real;
}

void Hydroacoustics::parseBuffer()
{
    qDebug() << "parseBuffer";
    int count = ha_buffer.count(36);
    qDebug() << count;
    while (count !=0)
    {
        count = count-1;
        qint8 index = ha_buffer.indexOf(36); //поиск индекса $
        if (index == -1)
        {
            // Не найдено сообщение
            qDebug() << "Нет сообщения в буфере";
            return;
        }
        qint8 crc_in = ha_buffer.indexOf(42);
        uint end = crc_in + 5; //последний символ посылки
        QByteArray title = ha_buffer.mid (index+1, 5);
        if (title == "PUWV0")
        {
            QByteArray msg = ha_buffer.mid(index+1, crc_in-1);
            if (crc_real(crc_in) == crc(msg))
            {
                parsePUWV0(msg);
//                updateData(uwave);
                ha_buffer.remove(0, end);
            }
            else
            {
                qDebug () << "PUWV0 crc не верна";
                ha_buffer.remove(0, end);
            }
        }
        if (title == "PUWVE")
        {
            QByteArray msg = ha_buffer.mid(index+1, crc_in-1);
            if (crc_real(crc_in) == crc(msg))
            {
                parsePUWVE(msg);
//                updateData(uwave);
                ha_buffer.remove(0, end);
            }
            else
            {
                qDebug () << "PUWVE crc не верна";
                ha_buffer.remove(0, end);
            }
        }
        if (title == "PUWV7")
        {
            QByteArray msg = ha_buffer.mid(index+1, crc_in-1);
            if (crc_real(crc_in) == crc(msg))
            {
                parsePUWV7(msg);
//                updateData(uwave);
                ha_buffer.remove(0, end);
            }
            else
            {
                qDebug () << "PUWV7 crc не верна";
                ha_buffer.remove(0, end);
            }
        }
        if (title == "PUWV4")
        {
            QByteArray msg = ha_buffer.mid(index+1, crc_in-1);
            if (crc_real(crc_in) == crc(msg))
            {
                parsePUWV4(msg);
//                updateData(uwave);
                ha_buffer.remove(0, end);
            }
            else
            {
                qDebug () << "PUWV4 crc не верна";
                ha_buffer.remove(0, end);
            }
        }
        if (title == "PUWV3")
        {
            QByteArray msg = ha_buffer.mid(index+1, crc_in-1);
            if (crc_real(crc_in) == crc(msg))
            {
                parsePUWV3(msg);
//                updateData(uwave);
                ha_buffer.remove(0, end);
            }
            else
            {
                qDebug () << "PUWV3 crc не верна";
                ha_buffer.remove(0, end);
            }
        }

    }
    int size = ha_buffer.size();
    ha_buffer.remove(0, size);
    updateData(uwave);

}

void Hydroacoustics::parsePUWV0(QByteArray msg)
{
    int index =msg.indexOf(44);//ищем первую запятую, перед ней идет не интересный заголовок
    msg.remove(0, index+1); // удаляем заголовок
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv0.cmdID = atoi(msg.mid(0, 1));
    qDebug() << "puwv0.cmdID: "<< uwave.puwv0.cmdID;
    msg.remove(0, index+1);
    index =msg.indexOf(42);//ищем звездочку *
    uwave.puwv0.errCode = atoi(msg.mid(0, 1));
    qDebug() << "puwv0.errCode: "<< uwave.puwv0.errCode;
    msg.remove(0, index+1);
    if (uwave.puwv0.cmdID == 2) uwave.puwv0.counterAnswer2++;
}

void Hydroacoustics::parsePUWVE(QByteArray msg)
{
    qDebug() << msg;
    int index =msg.indexOf(44);//ищем первую запятую, перед ней идет не интересный заголовок
    msg.remove(0, index+1); // удаляем заголовок
    index =msg.indexOf(44);//ищем запятую
    uwave.puwve.isPTMode = atoi(msg.mid(0, 1));
    qDebug() << "puwve.isPTMode: "<< uwave.puwve.isPTMode;
    msg.remove(0, index+1);
    index =msg.indexOf(42);//ищем звездочку *
    uwave.puwve.ptLocalAddress = atoi(msg.mid(0, 1));
    qDebug() << "puwve.ptLocalAddress: "<< uwave.puwve.ptLocalAddress;
    msg.remove(0, index+1);
}

void Hydroacoustics::parsePUWV7(QByteArray msg)
{
    qDebug() << msg;
    int index =msg.indexOf(44);//ищем первую запятую, перед ней идет не интересный заголовок
    msg.remove(0, index+1); // удаляем заголовок
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv7.Pressure_mBar = atof(msg.mid(0, index));
    qDebug() << "puwv7.Pressure_mBar: " << uwave.puwv7.Pressure_mBar;
    msg.remove(0, index+1);
    qDebug() << msg;
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv7.Temperature_C = atof(msg.mid(0, index));
    qDebug() << "puwv7.Temperature_C: " << uwave.puwv7.Temperature_C;
    msg.remove(0, index+1);
    qDebug() << msg;
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv7.Depth_m = atof(msg.mid(0, index));
    qDebug() << "puwv7.Depth_m: " << uwave.puwv7.Depth_m;
    msg.remove(0, index+1);
    qDebug() << msg;
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv7.VCC_V = atof(msg.mid(0, index));
    qDebug() << "puwv7.VCC_V: " << uwave.puwv7.VCC_V;
    msg.remove(0, index+1);
}

void Hydroacoustics::parsePUWV4(QByteArray msg)
{
    qDebug() << msg;
    int index =msg.indexOf(44);//ищем первую запятую, перед ней идет не интересный заголовок
    msg.remove(0, index+1); // удаляем заголовок
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv4.txChID = atoi(msg.mid(0, 1));
    qDebug() << "puwv4.txChID: "<< uwave.puwv4.txChID;
    msg.remove(0, index+1);
    index =msg.indexOf(42);//ищем звездочку *
    uwave.puwv4.rcCmdID = atoi(msg.mid(0, 1));
    qDebug() << "puwv4.rcCmdID: "<< uwave.puwv4.rcCmdID;
    msg.remove(0, index+1);
    uwave.puwv4.counter++;
}

void Hydroacoustics::parsePUWV3(QByteArray msg)
{
    qDebug() << msg;
    int index =msg.indexOf(44);//ищем первую запятую, перед ней идет не интересный заголовок
    msg.remove(0, index+1); // удаляем заголовок
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv3.txChID = atoi(msg.mid(0, 1));
    qDebug() << "puwv3.txChID: "<< uwave.puwv3.txChID;
    msg.remove(0, index+1);
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv3.rcCmdID = atoi(msg.mid(0, 1));
    qDebug() << "puwv3.rcCmdID: "<< uwave.puwv3.rcCmdID;
    msg.remove(0, index+1);
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv3.propTime = atof(msg.mid(0, index));
    qDebug() << "puwv3.propTime: "<< uwave.puwv3.propTime;
    msg.remove(0, index+1);
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv3.MSR = atof(msg.mid(0, index));
    qDebug() << "puwv3.MSR: "<< uwave.puwv3.MSR;
    msg.remove(0, index+1);
    index =msg.indexOf(44);//ищем запятую
    uwave.puwv3.Value = atof(msg.mid(0, index));
    qDebug() << "puwv3.Value: "<< uwave.puwv3.Value;
    msg.remove(0, index+1);
    uwave.puwv3.counter++;
}
