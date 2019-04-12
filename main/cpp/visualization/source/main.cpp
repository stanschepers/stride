/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2019, Jan Broeckhove and Bistromatics group.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <iostream>

#include "MapController.h"

#include <iostream>

int main(int argc, char* argv[])
{
        //    std::string filename = argv[0];
        visualization::MapController ctrl("/home/laurens/Desktop/test.json");

        QGuiApplication app(argc, argv);

        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("ctrl", &ctrl);
        engine.load(QUrl::fromLocalFile("./bin/Qt5Stride/main.qml"));

        QObject* root = engine.rootObjects()[0];
        ctrl.initialize(root);

        return app.exec();
}