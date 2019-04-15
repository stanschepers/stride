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

#include <QVariant>

#include "geopop/io/EpiOutputReaderFactory.h"
#include "MapController.h"

namespace visualization {

MapController::MapController(const std::string& filename) : QObject(nullptr)
{
        // Read in the epi-output
        visualization::EpiOutputReaderFactory readerFactory;
        const auto& reader = readerFactory.CreateReader(filename, m_epiOutput);
        reader->Read();
        // Check how big the gap in between measurements is
        auto diff = m_epiOutput[0].epiOutput.begin()->second.begin()->second.begin();
        diff++;
        m_day_diff = diff->first;
}

void MapController::setDay(const QString& day)
{
        auto temp_day = static_cast<unsigned int>(day.toDouble());
        temp_day      = (temp_day / m_day_diff) * m_day_diff;
        if (temp_day != m_day) {
                m_day = temp_day;
                std::cout << m_day << std::endl;
        }
}

void MapController::initialize(QObject* root)
{
        m_root = root;

        // Set the current day as the first day
        m_day = m_epiOutput[0].epiOutput.begin()->second.begin()->second.begin()->first;

        // Variables for the slider
        unsigned int firstDay = m_day;
        unsigned int lastDay  = m_epiOutput[0].epiOutput.begin()->second.begin()->second.rbegin()->first;

        // Variables for the map
        unsigned int zoomlevel       = 10; // TODO: Hoe zoomlevel uitrekenen?
        double       centerLatitude  = 0;
        double       centerLongitude = 0;
        for (auto const& location : m_epiOutput) {
                // To calculate the center of the map
                centerLatitude += location.latitude;
                centerLongitude += location.longitude;

                // Create an id for the circle on the map
                QString id = QString::fromStdString("location" + std::to_string(location.id));
                // Add the circle to the map
                // TODO: Hoe straal berekenen?
                QMetaObject::invokeMethod(root, "addLocation", Q_ARG(QVariant, id),
                                          Q_ARG(QVariant, QVariant::fromValue(location.latitude)),
                                          Q_ARG(QVariant, QVariant::fromValue(location.longitude)),
                                          Q_ARG(QVariant, QVariant::fromValue(location.pop_count * 100)));
        }

        centerLatitude /= m_epiOutput.size();
        centerLongitude /= m_epiOutput.size();
        QMetaObject::invokeMethod(
            root, "initialize", Q_ARG(QVariant, QVariant::fromValue(zoomlevel)),
            Q_ARG(QVariant, QVariant::fromValue(centerLatitude)), Q_ARG(QVariant, QVariant::fromValue(centerLongitude)),
            Q_ARG(QVariant, QVariant::fromValue(firstDay)), Q_ARG(QVariant, QVariant::fromValue(lastDay)));
}

void MapController::update() const
{
        //    for( auto const& location : m_epiOutput )
        //    {
        //        for( auto const& [ageBracketKey, ageBracketVal] : location.epiOutput)
        //        {
        //            for( auto const& [healthStatusKey, healthStatusVal] : ageBracketVal )
        //            {
        //
        //            }
        //        }
        //    }
}

} // namespace visualization