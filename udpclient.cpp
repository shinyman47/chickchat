#include "udpclient.h"
#include <FelgoApplication>
#include <QUdpSocket>
#include <QHostInfo>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QFile>

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
    qDebug()<<"set";

}
void UdpClient::saveNameToJson(QJsonValue nekename)
{
    QFile file;
    if(nekename.isObject())
    {
        QJsonObject nameobj=nekename.toObject();
        QString strname=nameobj.value("name").toString();
        QString strPwd=nameobj.value("passwd").toString();
    }
    QJsonObject j;
    j.insert("name",nekename);
    QJsonDocument o;
    o.setObject(j);
    QTextStream(stdout)<<o.toJson();
    QTextStream(stdout)<<o.toJson(QJsonDocument::Compact);
    //u_name = nekename;
    //qDebug()<<u_name<<"set";
}

void UdpClient::setPsword(QJsonValue password)
{
    if(password.isObject())
    {
        QJsonObject nameobj=password.toObject();
        QString strname=nameobj.value("name").toString();
        QString strPwd=nameobj.value("passwd").toString();
    }
    QJsonObject j;
    j.insert("password",password);
    QJsonDocument o;
    o.setObject(j);
    QTextStream(stdout)<<o.toJson();
    QTextStream(stdout)<<o.toJson(QJsonDocument::Compact);
    //u_name = nekename;
    //qDebug()<<u_name<<"set";
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
void UdpClient::sndMsg(MsgType type,QString msg, QString usrName, QString ipAddr)
{
    qDebug()<<u_name<<"test";
    port = 2333;
    //qDebug()<<u_name;
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);    
    out<<type;
    //qDebug()<<getIP();
    switch (type) {
    case Msg:
        //if(){}
        out << usrName << getIP() << msg;
        qDebug()<<usrName<<ipAddr<<msg;
        udpSocket->writeDatagram(data, data.size(), QHostAddress::Broadcast, port);

        //send message ui->msgBrowser->verticalScrollBar()->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
        break;
    case UsrEnter:
        //out << address;
        break;
    case UsrLeft:
        break;
    case FileName:
        out << usrName << msg;
        udpSocket->writeDatagram(data, data.size(),QHostAddress(ipAddr), port);

        break;
    case Refuse:
        break;
    }
    port = 2333;
    //udpSocket->writeDatagram("0",QHostAddress::Broadcast, port);
    //udpSocket->writeDatagram(usr, usr.size(),QHostAddress::Broadcast, port);
    //udpSocket->writeDatagram(addrs,addrs.size(), QHostAddress::Broadcast, port);
    //udpSocket->writeDatagram(message,message.size(),QHostAddress::Broadcast, port);
}



void UdpClient::initalizeTcp()
{
    tcpPort=34234;
    bytesToWrite=0;
    this->tcpServer=new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(sendFile()));
    connect(tcpServer,SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this,SLOT(printMsg(QAbstractSocket::SocketError)));
    if(!tcpServer->listen(QHostAddress::Any,tcpPort)){
        qDebug()<<"TCP LISTEN ERROR";
        return;
    }
    else {

        qDebug()<<"TCP listening...";
    }
}

void UdpClient::setFileName(const QString &value)
{
    fileName = value;
    if(!fileName.isEmpty()){
        theFileName=fileName.right(fileName.size()-fileName.lastIndexOf('/')-1);
    }
    qDebug()<<theFileName;
}

void UdpClient::acceptAndConnect(QString friendIPv4)
{
    bytesReceived=0;
    fileNameSize=0;
    totalBytes=0;

    tcpPort=34234;
    this->r_blockSize=0;
    tcpSocketRec=new QTcpSocket(this);
    connect(tcpSocketRec,SIGNAL(readyRead()),this,SLOT(recFile()));
    tcpSocketRec->abort();
    tcpSocketRec->connectToHost(QHostAddress(friendIPv4),tcpPort);
//    tcpSocketRec->connectToHost(QHostAddress::LocalHost,tcpPort);
}

void UdpClient::setFullPath(QString dir)
{
    this->r_path=dir;
    //wait to recived filename
}

void UdpClient::sendFile()
{
    // send file's total size, filename
    tcpSocketSend=tcpServer->nextPendingConnection();
    connect(tcpSocketSend,SIGNAL(bytesWritten(qint64)),this,SLOT(SendContinueAndUpdateProgressBar(qint64)));
    locFile=new QFile(fileName);
    if(!locFile->open(QFile::ReadOnly)){
        qDebug()<<"file open fail";
        return ;
    }
    totalBytesToSend=locFile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_7);
    sendOut<<qint64(0)<<qint64(0)<<theFileName;
    this->totalBytesToSend+=outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<totalBytesToSend<<qint64(outBlock.size()-sizeof(qint64)*2);
    bytesToWrite=totalBytesToSend-tcpSocketSend->write(outBlock);
    outBlock.resize(0);
}

void UdpClient::SendContinueAndUpdateProgressBar(qint64 numBytes)
{
    //send file data
    bytesWrriten+=(int)numBytes;
    if(bytesToWrite>0){
        outBlock=locFile->read(qMin(bytesToWrite,payloadSize));
        bytesToWrite-=(int)tcpSocketSend->write(outBlock);
        outBlock.resize(0);
    }
    else{
        locFile->close();
    }
    if(bytesWrriten==totalBytesToSend){
        locFile->close();
        tcpServer->close();
        emit fileStatus("Success");
    }
    emit updateProgressBar(bytesWrriten/totalBytesToSend);
}

void UdpClient::recFile()
{
    //reference
    QDataStream in(tcpSocketRec);
    in.setVersion(QDataStream::Qt_4_7);
    if(bytesReceived <= sizeof(qint64)*2)
    { //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息
        if((tcpSocketRec->bytesAvailable() >= sizeof(qint64)*2)&& (fileNameSize == 0))
        { //接收数据总大小信息和文件名大小信息
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
        if((tcpSocketRec->bytesAvailable() >= fileNameSize)
                && (fileNameSize != 0))
        {  //接收文件名，并建立文件
            in >> r_fileName;
            bytesReceived += fileNameSize;
            localFile = new QFile(r_path+'/'+ r_fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                qDebug() << "open file error!";
                return;
            }
        }
        else return;
    }
    if(bytesReceived < totalBytes)
    {  //如果接收的数据小于总数据，那么写入文件
        bytesReceived += tcpSocketRec ->bytesAvailable();
        inBlock = tcpSocketRec->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    if(bytesReceived == totalBytes)
    { //接收数据完成时
        tcpSocketRec->close();
        localFile->close();
        emit recSuccess();
    }
    emit updateRecBar(bytesReceived/totalBytes);
}

void UdpClient::printMsg(QAbstractSocket::SocketError socketError)
{

    qDebug()<<socketError;
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

        switch (msgType) {
        case Msg:
            in >> usrName >> ipAddr >> msg;

            //qDebug()<<usrName<<ipAddr<<msg;

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
            qDebug()<<"i had received your filemessage!!";

            in>>usrName;
            /*if(usrName != u_name){
                qDebug()<<usrName<<":NOT ME";
                return;
            }*/
            in>>msg;
            qDebug()<<"ready to send file";
            emit fileCome(usrName,ipAddr,msg);
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
QJsonValue UdpClient::getUsr()
{
    return u_name;

}
/*QString UdpClient::getMsg(QString msg)
{
    //QString msg =
    //get message from editline,clear editline,
    return msg;

}*/





