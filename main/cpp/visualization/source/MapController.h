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

#pragma once

#include "geopop/GeoGrid.h"

#include <QObject>
#include <QString>

#include "EpiOutput.h"

namespace visualization {

class MapController : public QObject
{

        Q_OBJECT
        Q_PROPERTY(QString shownDay READ getDay WRITE setDay NOTIFY dayChanged)
        Q_PROPERTY(QString setWindowHeight WRITE setWindowHeight NOTIFY heightChanged)
        Q_PROPERTY(QString setWindowWidth WRITE setWindowWidth NOTIFY widthChanged)
        Q_PROPERTY(QString setShownInformation WRITE setShownInformation NOTIFY tempChanged)

public:
        MapController() = default;

        /// Create a MapController and read the epi-output file
        explicit MapController(const std::string& filename);

        /// Set the shown day
        void setDay(const QString& day);

        /// Get the shown day
        QString getDay();

        /// Set the window height
        void setWindowHeight(const QString& height);

        /// Set the window width
        void setWindowWidth(const QString& width);

        /// Set the shown information about one locality
        void setShownInformation(const QString& locationId);

        /// Initialize the map
        void initialize(QObject* root);

signals:
        void dayChanged();

        void heightChanged();

        void widthChanged();

        void tempChanged();

private:

        /// Update the circles/information on the map
        void update() const;

        geopop::GeoGrid<EpiOutput> m_geogrid;

        unsigned int m_day = 0;      // Current shown step
        unsigned int m_day_diff = 0; // Difference between epi-output measurements (Amount of days)
        unsigned int m_window_height = 0;
        unsigned int m_window_width = 0;

        QObject* m_root = nullptr;
};

} // namespace visualization
