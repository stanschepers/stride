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
 *  Copyright 2019, Laurens Van Damme.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QFileInfo>

#include <iostream>

#include "MapController.h"

/// Main program of the stride visualizer.
int main(int argc, char* argv[])
{
        // -----------------------------------------------------------------------------------------
        // Check if an epi-output file is given
        // -----------------------------------------------------------------------------------------
        if (argc == 2){
                QFileInfo check_file(argv[1]);
                // check if file exists and if yes: Is it really a file and no directory?
                if (!check_file.exists() || !check_file.isFile()) {
                        std::cerr << "The given file doesn't exist!" << std::endl;
                        return -1;
                }
        }
        else if (argc < 2){
                std::cerr << "No epi-output file specified!" << std::endl;
                return -1;
        }
        else {
                std::cerr << "To many arguments were given!" << std::endl;
                return -1;
        }

        // -----------------------------------------------------------------------------------------
        // Run the Qt visualization tool.
        // -----------------------------------------------------------------------------------------
        visualization::MapController ctrl(argv[1]);

        QGuiApplication app(argc, argv);

        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("ctrl", &ctrl);
        engine.load(QUrl::fromLocalFile("./bin/Qt5Stride/main.qml"));

        QObject* root = engine.rootObjects()[0];
        ctrl.initialize(root);

        return app.exec();
}