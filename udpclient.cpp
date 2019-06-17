#include"udpclient.h"
#include <QUdpSocket>
#include <QHostInfo>
//#include <QMessageBox>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include<iostream>

UdpClient::UdpClient()
{
    //SocketSetting();
}

void UdpClient::test()
{
    std::cout<<"hello";

}
void UdpClient::socketSetting()
{
    //uname = usrName;
    udpSocket = new QUdpSocket(this);
    port = 23232;

    udpSocket->bind(port, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    //sndMsg(UsrEnter);
}

void UdpClient::setName(QString nekename)
{

    u_name = nekename;

}

/*void UdpClient::UsrEnter(QString usrName, QString ipAddr)
{
    //UdpClient::sndMsg(UsrEnter);
}*/

//void usrLeft(QString usrName, QString time)
//{}

void UdpClient::sndMsg(QString msg)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    QString address = getIP();
    out << getUsr();
    out << address << msg;
    /*switch (type) {
    case Msg:
        //if(){}

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

    }*/
    udpSocket->writeDatagram(data,data.length(), QHostAddress::Broadcast, port);
}

void UdpClient::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream in(&datagram, QIODevice::ReadOnly);
        int msgType;
        //in >> msgType;
        QString usrName,ipAddr,msg;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-ddhh:mm:ss");
        in >> usrName >> ipAddr >> msg;
        emit signalMsg(usrName, ipAddr, msg);
        /*switch (msgType) {
        case Msg:


            //recve message
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
        }*/

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


