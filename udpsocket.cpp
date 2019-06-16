#include"udpsocket.h"
#include <QUdpSocket>
#include <QHostInfo>
//#include <QMessageBox>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>


UdpSocket::UdpSocket()
{}


void UdpSocket::SocketSetting()
{
    //uname = usrName;
    udpSocket = new QUdpSocket(this);
    port = 23232;
    udpSocket->bind(port, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    //sndMsg(UsrEnter);
}

void UdpSocket::setName(QString nekename)
{
    u_name = nekename;
}

/*void UdpSocket::UsrEnter(QString usrName, QString ipAddr)
{
    //UdpSocket::sndMsg(UsrEnter);
}*/

//void usrLeft(QString usrName, QString time)
//{}

void UdpSocket::sndMsg(MsgType type, QString msg)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    QString address = getIP();
    out << type << getUsr();
    switch (type) {
    case Msg:
        //if(){}
        out << address << msg;
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
    udpSocket->writeDatagram(data,data.length(), QHostAddress::Broadcast, port);
}

void UdpSocket::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream in(&datagram, QIODevice::ReadOnly);
        int msgType;
        in >> msgType;
        QString usrName,ipAddr,msg;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-ddhh:mm:ss");
        switch (msgType) {
        case Msg:
            in >> usrName >> ipAddr >> msg;
            //recve message
            break;
        case UsrEnter:
            in >> usrName >> ipAddr;
           // UsrEnter(usrName, ipAddr);
            break;
        case UsrLeft:
            in >> usrName;
           // usrLeft(usrName, time);
            break;
        case FileName:
            break;
        case Refuse:
            break;
        }

    }

}
QString UdpSocket::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress addr, list){
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return 0;
}
QString UdpSocket::getUsr()
{
    return u_name;

}
/*QString UdpSocket::getMsg(QString msg)
{
    //QString msg =
    //get message from editline,clear editline,
    return msg;

}*/


