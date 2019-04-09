#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

#include "execs/StanController.h"

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace boost::property_tree;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("./bin/Qt5Stride/main.qml")));

    return app.exec();
}