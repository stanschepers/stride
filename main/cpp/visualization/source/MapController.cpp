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

#include <cmath>
#include <limits>

#include <QVariant>

#include "EpiOutputReaderFactory.h"
#include "MapController.h"

namespace visualization {

double distanceOnEarth(double lat1, double long1, double lat2, double long2){
//        double a = std::pow((std::sin((lat2 - lat1) / 2) * M_PI) / 180, 2) + std::cos((lat1 * M_PI) / 180) * std::cos((lat2 * M_PI) / 180) * std::pow((std::sin((long2 - long1) / 2) * M_PI) / 180, 2);
//        a = 2 * 6371000 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
        double a = std::acos(std::sin((lat1 * M_PI) / 180) * std::sin((lat2 * M_PI) / 180) + std::cos((lat1 * M_PI) / 180) * std::cos((lat2 * M_PI) / 180) * std::cos(((long2 - long1) * M_PI) / 180)) * 6731000;
//        std::cout << a << std::endl;
        return a;
}

MapController::MapController(const std::string& filename) : QObject(nullptr), m_geogrid(geopop::GeoGrid<EpiOutput>(nullptr))
{
        // Read in the epi-output
        visualization::EpiOutputReaderFactory readerFactory;
        const auto& reader = readerFactory.CreateReader(filename, &m_geogrid);
        reader->Read();
        // Check how big the gap is in between measurements
        auto diff = m_geogrid[0]->getContent()->epiOutput.begin()->second.begin()->second.begin();
        diff++;
        m_day_diff = diff->first;
}

void MapController::setDay(const QString& day)
{
        auto temp_day = static_cast<unsigned int>(day.toDouble());
//        std::cout << "Given day: " << temp_day << std::endl;
        temp_day      = (temp_day / m_day_diff) * m_day_diff;
        if (temp_day != m_day) {
                m_day = temp_day;
//                std::cout << "Day set to: " << m_day << std::endl;
        }
}

QString MapController::getDay(){
    return QString::number(m_day);
}

    void MapController::setWindowHeight(const QString& height){
        m_window_height = static_cast<unsigned int>(height.toDouble());
}

void MapController::setWindowWidth(const QString& width){
        m_window_width = static_cast<unsigned int>(width.toDouble());
}

void MapController::setShownInformation(const QString& locationId){
        if (locationId == QString("")){
                QMetaObject::invokeMethod(m_root, "emptyData");
                return;
        }
        for (const auto &location: m_geogrid){
                if (location->GetID() == locationId.toUInt()){
                        QMetaObject::invokeMethod(m_root, "setData", Q_ARG(QVariant, QString::fromStdString(location->GetName())));
                        return;
                }
        }

}

void MapController::initialize(QObject* root)
{
        m_root = root;

        // Set the current day as the first day
        m_day = m_geogrid[0]->getContent()->epiOutput.begin()->second.begin()->second.begin()->first;

        // Variables for the slider
        unsigned int firstDay = m_day;
        unsigned int lastDay  = m_geogrid[0]->getContent()->epiOutput.begin()->second.begin()->second.rbegin()->first;

        // Variables for the map
        double zoomlevel        = 0;
        double centerLatitude   = 0;
        double centerLongitude  = 0;
        double smallestLat      = std::numeric_limits<double>::infinity();;
        double biggestLat       = 0;
        double smallestLong     = std::numeric_limits<double>::infinity();;
        double biggestLong      = 0;

//        // Sort the locations in order of population size (Great to small)
//        std::sort(m_geogrid.begin(), m_geogrid.end(), [ ]( const std::shared_ptr<geopop::Location<EpiOutput>>& lhs, const std::shared_ptr<geopop::Location<EpiOutput>>& rhs )
//        {
//            return lhs->getContent()->pop_count > rhs->getContent()->pop_count;
//        });

        // Put the locations on the map
        for (auto const& location : m_geogrid) {
                
                // To calculate the zoom level
                if (location->GetCoordinate().get<0>() < smallestLat){
                    smallestLat = location->GetCoordinate().get<0>();
                }
                if (location->GetCoordinate().get<0>()  > biggestLat){
                    biggestLat = location->GetCoordinate().get<0>();
                }
                if (location->GetCoordinate().get<1>() < smallestLong){
                    smallestLong = location->GetCoordinate().get<1>();
                }
                if (location->GetCoordinate().get<1>()  > biggestLong){
                    biggestLong = location->GetCoordinate().get<1>();
                }

                // Add the circle to the map
                QMetaObject::invokeMethod(m_root, "addLocation", Q_ARG(QVariant, QString::number(location->GetID())),
                                          Q_ARG(QVariant, QVariant::fromValue(location->GetCoordinate().get<0>())),
                                          Q_ARG(QVariant, QVariant::fromValue(location->GetCoordinate().get<1>())),
                                          Q_ARG(QVariant, QVariant::fromValue(location->getContent()->pop_count * 0.03)));  // radius
        }

        // Calculate the center of the map
        centerLatitude = (smallestLat + biggestLat) / 2;
        centerLongitude = (smallestLong + biggestLong) / 2;

        // Calculate the zoom level of the map
//        std::cout << smallestLat << "   " << biggestLat << std::endl;
//        std::cout << smallestLong << "   " << biggestLong << std::endl;
        double pixelsize = std::max(distanceOnEarth(smallestLat, smallestLong, smallestLat, biggestLong) / double(m_window_width), distanceOnEarth(smallestLat, smallestLong,biggestLat, smallestLong) / double(m_window_height));
        zoomlevel = (std::log2((40075016.686 * std::cos((centerLatitude * M_PI) / 180)) / pixelsize) - 8) * 0.9;
//        std::cout << pixelsize << "   " << zoomlevel << std::endl;

        // Change the properties of the map
        QMetaObject::invokeMethod(
            m_root, "initialize", Q_ARG(QVariant, QVariant::fromValue(zoomlevel)),
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