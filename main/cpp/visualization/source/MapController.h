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
        Q_PROPERTY(QString shownDay READ GetDay WRITE SetDay NOTIFY DayChanged)
        Q_PROPERTY(QString setWindowHeight READ GetDay WRITE SetWindowHeight NOTIFY HeightChanged)
        Q_PROPERTY(QString setWindowWidth READ GetDay WRITE SetWindowWidth NOTIFY WidthChanged)
        Q_PROPERTY(QString setShownInformation READ GetDay WRITE SetShownInformation NOTIFY ShownInformationChanged)
        Q_PROPERTY(QString setAgeBracket READ GetDay WRITE SetAgeBracket NOTIFY AgeBracketChanged)
        Q_PROPERTY(QString setHealthStatus READ GetDay WRITE SetHealthStatus NOTIFY HealthStatusChanged)
        Q_PROPERTY(bool dataPinned READ IsDataPinned WRITE PinData NOTIFY DataPinnedChanged)

public:
        /// Create a MapController and read the epi-output file
        explicit MapController(const std::string& filename);

        /// No copy constructor.
        MapController(const MapController&) = delete;

        /// No copy assignement.
        MapController& operator=(const MapController&) = delete;

        /// Set the shown day
        void SetDay(const QString &day);

        /// Get the shown day
        QString GetDay();

        /// Set the window height
        void SetWindowHeight(const QString &height);

        /// Set the window width
        void SetWindowWidth(const QString &width);

        /// Set the shown information about one locality
        void SetShownInformation(const QString &sourceInformation);

        /// Initialize the map
        void Initialize(QObject *root);

        /// Update the color of the circles on the map
        void UpdateLocations();

        /// Set the selected age bracket
        void SetAgeBracket(const QString &ageBracket);

        /// Set the selected health status
        void SetHealthStatus(const QString &healthStatus);

        /// Get the state of dataPinned
        bool IsDataPinned();

        /// Set the state of dataPinned
        void PinData(bool pinned);

signals:
        /// Signal for when day changes
        void DayChanged();

        /// Signal for when height changes
        void HeightChanged();

        /// Signal for when width changes
        void WidthChanged();

        /// Signal for when the show information changes
        void ShownInformationChanged();

        /// Signal for when the selected age bracket changes
        void AgeBracketChanged();

        /// Signal for when the selected health status changes
        void HealthStatusChanged();

        /// Signal for when the selected health status changes
        void DataPinnedChanged();

private:
        geopop::GeoGrid<EpiOutput> m_geogrid; ///< GeoGrid that contains all the locations and their epi-output

        bool m_data_pinned; ///< Indicates if the shown data is pinned
        unsigned int m_day;      ///<  Current shown step
        unsigned int m_day_diff; ///<  Difference (Amount of days) between epi-output measurements (Stride)
        unsigned int m_window_height; ///< Height of the GUI window
        unsigned int m_window_width; ///< Width of the GUI window
        std::string m_selectedAgeBracket; ///< Selected age bracket in the GUI
        std::string m_selectedHealthStatus; ///< Selected health status in the GUI
        std::map<std::string, std::map<std::string, double>> m_smallest_values; ///< Smallest epi-output values for each category
        std::map<std::string, std::map<std::string, double>> m_biggest_values; ///< Biggest epi-output values for each category

        QObject* m_root = nullptr; ///< Root of the Qt object
        QString m_pinned_data; ///< Information of the source of the last shown data
};

} // namespace visualization
