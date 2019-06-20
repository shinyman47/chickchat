#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include<QObject>
#include<QUdpSocket>
#include<QList>
#include<FelgoApplication>
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonValue>
#include<QTcpServer>
#include<QTcpSocket>
//#include<QFile>
#include<QString>
//#include<string>

class UdpClient :public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_ENUMS(MsgType)
public:
    //udp
    UdpClient(QObject *parent = nullptr);
    enum MsgType{Msg, UsrEnter, UsrLeft, FileName, Refuse};
    Q_INVOKABLE void test();
    //Q_INVOKABLE void socketSetting();
    Q_INVOKABLE void setName(QString nekename);
    Q_INVOKABLE void saveNameToJson(QJsonValue nekename);
    Q_INVOKABLE void setPsword(QJsonValue password);
    //Q_INVOKABLE void UsrEnter(QString usrName, QString ipAddr);
   // Q_INVOKABLE void usrLeft(QString usrName, QString time);
    Q_INVOKABLE void sndMsg(MsgType type,QString msg, QString usrName, QString ipAddr);
    //tcp
    Q_INVOKABLE void initalizeTcp();
    Q_INVOKABLE void setFileName(const QString &value);
    Q_INVOKABLE void acceptAndConnect(QString ipAddr);
    Q_INVOKABLE void setFullPath(QString dir);
    //getter
    Q_INVOKABLE QString getIP();
    Q_INVOKABLE QJsonValue getUsr();
   // Q_INVOKABLE QString getMsg(QString msg);
protected:
   // QString name() const{return u_name;}

private:
    //udp
    QUdpSocket *udpSocket;
    qint16 port;
    QString u_name;

    QString u_ipAddr;
    //tcp
    QTcpServer* tcpServer;
    qint16 tcpPort;
    QTcpSocket* tcpSocketSend;
    QString fileName; //path + name
    QString theFileName; //just name
    QFile* locFile; //
    qint64 totalBytesToSend;
    qint64 bytesToWrite;
    qint64 bytesWrriten;
    qint64 payloadSize;
    QByteArray outBlock;
    //tcp receiver
    qint64 r_blockSize;
    qint64 fileNameSize;
    qint64 totalBytes;
    QTcpSocket* tcpSocketRec;
    qint64 bytesReceived;
    QString r_fileName;
    QString r_path;
    QFile* localFile;
    QByteArray inBlock;
signals:
    //udp
    void nameChanged();
    void signalEnter(QString usrName, QString ipAddr);
    void signalLeft(QString usrName, QString time);
    void signalMsg(QString usrName,QString ipAddr ,QString msg);
    //tcp
    //tcp
    void fileCome(QString friendName,QString ipAddr,QString filename);
    void fileStatus(QString status);
    void updateProgressBar(double value);
    void updateRecBar(double value);
    void updateChatView();
    void recSuccess();
    void friendExit(int index);
private slots:
    void processPendingDatagrams();
    //tcp
    void sendFile();
    void SendContinueAndUpdateProgressBar(qint64 numBytes);
    void recFile();
    void printMsg(QAbstractSocket::SocketError socketError);
};

#endif // UDPCLIENT_H
