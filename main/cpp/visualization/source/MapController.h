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

/**
 * MapController for use within the visualization, controller of the GUI and communicates with the qml.
 */
class MapController : public QObject
{

        Q_OBJECT
        Q_PROPERTY(QString shownDay READ getDay WRITE setDay NOTIFY dayChanged)
        Q_PROPERTY(QString setWindowHeight READ getDay WRITE setWindowHeight NOTIFY heightChanged)
        Q_PROPERTY(QString setWindowWidth READ getDay WRITE setWindowWidth NOTIFY widthChanged)
        Q_PROPERTY(QString setShownInformation READ getDay WRITE setShownInformation NOTIFY shownInformationChanged)
        Q_PROPERTY(QString setAgeBracket READ getDay WRITE setAgeBracket NOTIFY ageBracketChanged)
        Q_PROPERTY(QString setHealthStatus READ getDay WRITE setHealthStatus NOTIFY healthStatusChanged)

public:
        /// Create a MapController and read the epi-output file
        explicit MapController(const std::string& filename);

        /// No copy constructor.
        MapController(const MapController&) = delete;

        /// No copy assignement.
        MapController& operator=(const MapController&) = delete;

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

        /// Update the color of the circles on the map
        void updateLocations();

        /// Set the selected age bracket
        void setAgeBracket(const QString& ageBracket);

        /// Set the selected health status
        void setHealthStatus(const QString& healthStatus);

signals:
        /// Signal for when day changes
        void dayChanged();

        /// Signal for when height changes
        void heightChanged();

        /// Signal for when width changes
        void widthChanged();

        /// Signal for when the show information changes
        void shownInformationChanged();

        /// Signal for when the selected age bracket changes
        void ageBracketChanged();

        /// Signal for when the selected health status changes
        void healthStatusChanged();

private:
        geopop::GeoGrid<EpiOutput> m_geogrid; ///< GeoGrid that contains all the locations and their epi-output

        unsigned int m_day;      ///<  Current shown step
        unsigned int m_day_diff; ///<  Difference (Amount of days) between epi-output measurements (Stride)
        unsigned int m_window_height; ///< Height of the GUI window
        unsigned int m_window_width; ///< Width of the GUI window
        std::string m_selectedAgeBracket; ///< Selected age bracket in the GUI
        std::string m_selectedHealthStatus; ///< Selected health status in the GUI
        std::map<std::string, std::map<std::string, double>> m_smallest_values; ///< Smallest epi-output values for each category
        std::map<std::string, std::map<std::string, double>> m_biggest_values; ///< Biggest epi-output values for each category

        QObject* m_root = nullptr; ///< Root of the Qt object
};

} // namespace visualization
