#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <iostream>

//#include "MapController.h"


int main(int argc, char *argv[]) {
    std::string filename = argv[0];

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("./bin/Qt5Stride/main.qml"));

    QObject *root = engine.rootObjects()[0];

//    MapController ctrl;
//    ctrl.addLocation(50.8503, 4.3517, 3000);
    QMetaObject::invokeMethod(root, "addLocation", Q_ARG(QVariant, QVariant::fromValue(50.8503)),
                              Q_ARG(QVariant, QVariant::fromValue(4.3517)), Q_ARG(QVariant, QVariant::fromValue(3000)));

    return app.exec();
}