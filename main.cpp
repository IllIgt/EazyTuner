#include "MainWindow.h"
#include <QQmlComponent>
#include <QApplication>
#include <QtQml/QQmlApplicationEngine>
#include "Recorder.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<Recorder>("Recorder", 1, 0, "Recorder");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/tuner/qml/MainWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
