#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include "execs/StanController.h"

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace boost::property_tree;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc,argv);
    QQuickView view;
//    ptree configPt;
//    StanController ctrl(configPt);
    view.setSource(QUrl(QStringLiteral("./bin/Qt5Stride/main.qml")));
//    view.setWidth(360);
//    view.setHeight(640);
//    view.show();
    return app.exec();
}