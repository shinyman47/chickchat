#include "udpclient.h"
#include <iostream>
#include <FelgoApplication>
#include <QUdpSocket>
#include <QHostInfo>
#include <QDateTime>
#include <QList>
#include <QTextStream>
#include <QNetworkInterface>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QJsonParseError>
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
    this->u_name = nekename;
    //qDebug()<<"set";

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
    //qDebug()<<u_name<<"test";
    port = 2333;
    //qDebug()<<u_name;
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);    
    out<<type;

    //QString usrname = "client";
    //!!!!!!!!!!!!!!!usrName is not true
    switch (type) {
    case Msg:
        //if(){}
        ipAddr = getIP().toString();
        //qDebug()<<ipAddr
        out << usrName << ipAddr << msg;

        udpSocket->writeDatagram(data, data.size(), QHostAddress::Broadcast, port);

        //send message ui->msgBrowser->verticalScrollBar()->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
        break;
    case UsrEnter:
        //out << address;
        break;
    case UsrLeft:
        break;
    case FileName:

        out << usrName<< getIP()<< msg ;
        udpSocket->writeDatagram(data, data.size(),QHostAddress(ipAddr), port);

        break;
    case Refuse:
        //qDebug()<<"start refuse";
        //out << usrName<<msg<<ipAddr;
        udpSocket->writeDatagram(data, data.size(),QHostAddress(ipAddr), port);
        break;
    }

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

void UdpClient::acceptAndConnect(QString ipAddr)
{
    qDebug()<<"usr accept file sending";
    bytesReceived=0;
    fileNameSize=0;
    totalBytes=0;

    tcpPort=34234;
    this->r_blockSize=0;
    tcpSocketRec=new QTcpSocket(this);
    connect(tcpSocketRec,SIGNAL(readyRead()),this,SLOT(recFile()));
    tcpSocketRec->abort();
    tcpSocketRec->connectToHost(QHostAddress(ipAddr),tcpPort);
//    tcpSocketRec->connectToHost(QHostAddress::LocalHost,tcpPort);
}

void UdpClient::setFullPath(QString dir)
{
    this->r_path=dir;
    //wait to recived filename
}

void UdpClient::sendFile()
{
    qDebug()<<"start to open file";
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
    emit fileStatus("Success");
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
        //QString fileUsrName,fileIpAddr,fileMsg;
        in >> msgType;


        QString time = QDateTime::currentDateTime().toString("yyyy-MM-ddhh:mm:ss");
        std::string usr, addr, message;

        switch (msgType) {
        case Msg:

            in >> usrName >> ipAddr >> msg;

            qDebug()<<usrName<<ipAddr<<msg;

            emit signalMsg(usrName,ipAddr,msg);
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
            in >> usrName >> ipAddr >> msg;
            //qDebug()<<"ready to send file";
            //qDebug()<<usrName<<ipAddr<<msg<<"test";
            emit signalFileCome(usrName,ipAddr,msg);
            //qDebug()<<"i had received your filemessage!!";
            break;
        case Refuse:

            emit fileStatus("Refuse");
            break;
        }
    }
}
//getter
QHostAddress UdpClient::getIP()
{
   /* QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress addr, list){
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            return addr.toString();

    }
    return 0;*/
    foreach(const QHostAddress & hostAddress, QNetworkInterface::allAddresses())
        if(hostAddress != QHostAddress::LocalHost && hostAddress.toIPv4Address())
            return hostAddress;
    return QHostAddress::LocalHost;
}
QJsonValue UdpClient::getUsr()
{
    return this-> u_name;

}
/*QString UdpClient::getMsg(QString msg)
{
    //QString msg =
    //get message from editline,clear editline,
    return msg;

}*/
//JSON

void UdpClient::setInfo(QString name,QString password)
{
    if(!checkName(name))
    {UdpClient u;
    u.person.push_back(PersonInfo(name,password));
    QJsonArray array;
    QJsonObject root;
    QJsonDocument docunment;
    for(auto o=0;o<u.person.size();o++)
   {
      QJsonObject object;
qDebug()<<u.person[o].getname();
qDebug()<<u.person[o].getpassword();
       object.insert("name",u.person[o].getname());
        object.insert("password",u.person[o].getpassword());
       array.append(object);
    }
    root.insert("user",array);
    docunment.setObject(root);
   QByteArray byte_array=docunment.toJson(QJsonDocument::Compact);
   QString json_str(byte_array);
QTextStream(stdout)<<json_str;
    QFile file("./assets/data.json");
    if(!file.open(QIODevice::Append|QIODevice::ReadWrite))
      {
        qDebug()<<"error";
      }
     QTextStream in(&file);
       in<<json_str<<endl;
    //u->userInfo.append(json_str);
    file.close();//write data to data.json
}
    else {
        qDebug()<<"already cunzai!";
        changePassword(name,"hello");
    }
}


QString UdpClient::getnname()
{
    QFile file("Data.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
      {
        qDebug()<<"open error";
      }
     QByteArray Data=file.readAll();
     file.close();
     QJsonParseError jsonError;
     QJsonDocument jsonDoc(QJsonDocument::fromJson(Data,&jsonError));
     if(jsonError.error!=QJsonParseError::NoError)
     {
         qDebug()<<"json error";
     }
     QJsonObject rootobject=jsonDoc.object();
     QJsonObject user=rootobject.value("user").toObject();
     qDebug()<<user["name"].toString();
     return user["name"].toString();

}

bool UdpClient::checkInfo(QString nekname,QString password)
{
    QFile file1("./assets/data.json");
  if(!file1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
      qDebug()<<"error";
    }
 while(!file1.atEnd())
 {
   QByteArray line=file1.readLine();
     QString str(line);
QString json(str);
  QJsonParseError jsonerror;
  QJsonDocument doc = QJsonDocument::fromJson(json.toLatin1(), &jsonerror);
  if (!doc.isNull() && jsonerror.error == QJsonParseError::NoError)
  {
      if(doc.isObject())
      {
          QJsonObject object = doc.object();
          QStringList list = object.keys();
          QJsonObject::iterator it = object.begin();
          while(it != object.end())
          {
              switch(it.value().type())
              {
              case QJsonValue::String:
              {
                  qDebug() << "Type is string";
                  QJsonArray sub = it.value().toArray();
                  qDebug() << it.key() << "=" << it.value().toString();
                  break;
              }
              case QJsonValue::Array:
              {
                  QJsonArray sub = it.value().toArray();
                  QJsonObject objectt=sub.at(0).toObject();
                  if(nekname==objectt["name"].toString()&&password==objectt["password"].toString())
                   {
                     qDebug()<<"succeed!";
                  file1.close();//data.json
                   return true;
                  }break;
              }
              case QJsonValue::Bool:
              {
                  qDebug() << "Type is Bool";
                  qDebug() << it.key() << "=" << it.value().toBool();
                  break;
              }
              case QJsonValue::Double:
              {
                  qDebug() << "Type is Double";
                  qDebug() << it.key() << "=" << it.value().toDouble();
                  break;
              }
              case QJsonValue::Object:
              {
                  qDebug() << "Type is Object";
                  qDebug() << it.key() << "=" << it.value().toObject();
                  break;
              }
              case QJsonValue::Null:
              {
                  qDebug() << "Type is Null";
                  qDebug() << it.key() << "= NULL" ;
                  break;
              }
              case QJsonValue::Undefined:
              {
                  qDebug() << "Type is Undefined";
                  break;
              }
              }
              it++;
          }
      }
  }
 }
 file1.close();//data.json
 qDebug()<<"no you!";
 return false;

}
bool UdpClient::checkName(QString nekname)
{
    QFile file1("./assets/data.json");
  if(!file1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
      qDebug()<<"error";
    }
 while(!file1.atEnd())
 {
   QByteArray line=file1.readLine();
  QString str(line);
    QString json(str);
  QJsonParseError jsonerror;
  QJsonDocument doc = QJsonDocument::fromJson(json.toLatin1(), &jsonerror);
  if (!doc.isNull() && jsonerror.error == QJsonParseError::NoError)
  {
      if(doc.isObject())
      {
          QJsonObject object = doc.object();
          QStringList list = object.keys();
          QJsonObject::iterator it = object.begin();
          while(it != object.end())
          {
              switch(it.value().type())
              {
              case QJsonValue::String:
              {
                  qDebug() << "Type is string";
                  QJsonArray sub = it.value().toArray();
                  qDebug() << it.key() << "=" << it.value().toString();
                  break;
              }
              case QJsonValue::Array:
              {
                  QJsonArray sub = it.value().toArray();
                  QJsonObject objectt=sub.at(0).toObject();
                  if(nekname==objectt["name"].toString())//&&password==objectt["password"].toString())
                   {
                     qDebug()<<"succeed!";
                  file1.close();//data.json
                   return true;
                  }break;
              }
              case QJsonValue::Bool:
              {
                  qDebug() << "Type is Bool";
                  qDebug() << it.key() << "=" << it.value().toBool();
                  break;
              }
              case QJsonValue::Double:
              {
                  qDebug() << "Type is Double";
                  qDebug() << it.key() << "=" << it.value().toDouble();
                  break;
              }
              case QJsonValue::Object:
              {
                  qDebug() << "Type is Object";
                  qDebug() << it.key() << "=" << it.value().toObject();
                  break;
              }
              case QJsonValue::Null:
              {
                  qDebug() << "Type is Null";
                  qDebug() << it.key() << "= NULL" ;
                  break;
              }
              case QJsonValue::Undefined:
              {
                  qDebug() << "Type is Undefined";
                  break;
              }
              }
              it++;
          }
      }
  }
 }
 file1.close();//data.json
 qDebug()<<"no you!";
 return false;

}

bool UdpClient::changePassword(QString nekname,QString newpassword)
{
    QFile file1("./assets/data.json");
  if(!file1.open(QIODevice::ReadWrite|QIODevice::Text))
    {
      qDebug()<<"error";
    }
 while(!file1.atEnd())
 {
   QByteArray line=file1.readLine();
   QString str(line);
   QString json(str);
   qDebug()<<json.toLocal8Bit()<<endl;
  QJsonParseError jsonerror;
 auto const& doc = QJsonDocument::fromJson(json.toLatin1(), &jsonerror);
  if (!doc.isNull() && jsonerror.error == QJsonParseError::NoError)
  {
      if(doc.isObject())
      {
          QJsonObject object = doc.object();
          QStringList list = object.keys();
          QJsonObject::iterator it = object.begin();
          while(it != object.end())
          {
              switch(it.value().type())
              {
              case QJsonValue::String:
              {
                  qDebug() << "Type is string";
                  QJsonArray sub = it.value().toArray();
                  qDebug() << it.key() << "=" << it.value().toString();
                  break;
              }
              case QJsonValue::Array:
              {
                  QJsonValueRef arrayref=object.find("user").value();
                  QJsonArray sub = arrayref.toArray();
                  QJsonArray::iterator it=sub.begin();
                  QJsonValueRef element=it[0];
                  QJsonObject objectt=element.toObject();
                  if(nekname==objectt["name"].toString())
                   {
                      qDebug()<<"old password:"<< objectt["password"].toString();
                      objectt.insert("password",newpassword);
                      element=objectt;
                      arrayref=sub;
                      qDebug()<<"new password:"<< objectt["password"].toString();
                     qDebug()<<"change succeed!";
                  file1.close();//data.json
                   return true;
                  }break;
              }
              case QJsonValue::Bool:
              {
                  qDebug() << "Type is Bool";
                  qDebug() << it.key() << "=" << it.value().toBool();
                  break;
              }
              case QJsonValue::Double:
              {
                  qDebug() << "Type is Double";
                  qDebug() << it.key() << "=" << it.value().toDouble();
                  break;
              }
              case QJsonValue::Object:
              {
                  qDebug() << "Type is Object";
                  qDebug() << it.key() << "=" << it.value().toObject();
                  break;
              }
              case QJsonValue::Null:
              {
                  qDebug() << "Type is Null";
                  qDebug() << it.key() << "= NULL" ;
                  break;
              }
              case QJsonValue::Undefined:
              {
                  qDebug() << "Type is Undefined";
                  break;
              }
              }
              it++;
          }
      }
  }
 }
 file1.close();//data.json
 qDebug()<<"no you!";
 return false;

}





