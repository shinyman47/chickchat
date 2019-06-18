#include "udpclient.h"
#include <QUdpSocket>
#include <QHostInfo>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include <QVector>

UdpClient::UdpClient(QObject *parent)
:QObject(parent)
{
    port = 2333;
    udpSocket = new QUdpSocket;
    udpSocket->bind(port, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}
void UdpClient::test()
{
    QByteArray ba;
    while(udpSocket->hasPendingDatagrams())
    {
        ba.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(ba.data(), ba.size());
        //std::cout << ba.data() << std::endl;

        //qDebug()<<ba.data();
        //QDataStream in(&ba, QIODevice::ReadOnly);
        QVector<QString>array;
    }
}
/*void UdpClient::socketSetting()
{
    //uname = usrName;

    udpSocket = new QUdpSocket(this);
    port = 2333;
    udpSocket->bind(QHostAddress("127.0.0.1"),port);
    //udpSocket->bind(port, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    //sndMsg(UsrEnter);
}*/
void UdpClient::setName(QString nekename)
{
    u_name = nekename;
    qDebug()<<u_name<<"set";
}

/*void UdpClient::UsrEnter(QString usrName, QString ipAddr)
{
    //UdpClient::sndMsg(UsrEnter);
}*/

//void usrLeft(QString usrName, QString time)
//{}
/*void UdpClient::sndMsg(MsgType type,QString msg)
{
    QByteArray data = "hello";
    udpSocket->writeDatagram(data,data.size(),QHostAddress("127.0.0.1"),port);

}*/
void UdpClient::sndMsg(MsgType type,QString msg)
{

    //qDebug()<<u_name;
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    QString address = getIP();
    //QByteArray addrs = address.toLocal8Bit();
    //QByteArray message = msg.toLocal8Bit();
    //QByteArray usr = getUsr().toLocal8Bit();
    //QByteArray usr = "client";
    out<<type;
    QString usr = "client";

    //qDebug()<<u_name<<":"<<message<<"from"<<addrs;
    switch (type) {
    case Msg:
        //if(){}
        out << usr << address << msg;
        //send message ui->msgBrowser->verticalScrollBar()->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
        break;
    case UsrEnter:
        out << address;
        break;
    case UsrLeft:
        break;
    case FileName:
        break;
    case Refuse:
        break;
    }
    port = 2333;
    //udpSocket->writeDatagram("0",QHostAddress::Broadcast, port);
    //udpSocket->writeDatagram(usr, usr.size(),QHostAddress::Broadcast, port);
    //udpSocket->writeDatagram(addrs,addrs.size(), QHostAddress::Broadcast, port);
    //udpSocket->writeDatagram(message,message.size(),QHostAddress::Broadcast, port);
    udpSocket->writeDatagram(data, data.size(), QHostAddress::Broadcast, port);
}
void UdpClient::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        //qDebug()<<"successs";

        QByteArray datagram;

        datagram.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream in(&datagram, QIODevice::ReadOnly);

        int msgType;
        QString usrName,ipAddr,msg;
        in >> msgType;
        //qDebug()<<msgType<<usrName<<ipAddr<<msg;




        QString time = QDateTime::currentDateTime().toString("yyyy-MM-ddhh:mm:ss");
        std::string usr, addr, message;

        QString a ="jack";
        QString b ="198.0.0";
        QString c ="hello";

        switch (msgType) {
        case Msg:
            in >> usrName >> ipAddr >> msg;

            emit signalMsg(usrName,msg,ipAddr);
            break;
        case UsrEnter:
            in >> usrName >> ipAddr;
            emit signalEnter(usrName, ipAddr);
           // UsrEnter(usrName, ipAddr);
            break;
        case UsrLeft:
            in >> usrName;
            emit signalLeft(usrName,time);
           // usrLeft(usrName, time);
            break;
        case FileName:
            break;
        case Refuse:
            break;
        }
    }
}
QString UdpClient::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress addr, list){
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return 0;
}
QString UdpClient::getUsr()
{
    return u_name;

}
/*QString UdpClient::getMsg(QString msg)
{
    //QString msg =
    //get message from editline,clear editline,
    return msg;

}*/





