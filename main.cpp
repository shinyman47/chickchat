#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "udpclient.h"
#include <QtQml>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    qmlRegisterType<UdpClient>("client", 1, 0, "UdpClient");


    QQmlApplicationEngine engine;
    UdpClient* client=new UdpClient();

    engine.rootContext()->setContextObject(client);
    //QScopedPointer<UdpClient> udpclient (new UdpClient);
    //engine.rootContext()->setContextProperty("udpclient", udpclient.data());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
