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

#include "geopop/Location.h"
#include "geopop/Coordinate.h"

#include <fstream>
#include <memory>

#include "EpiOutputJSONReader.h"

namespace visualization {

EpiOutputJSONReader::EpiOutputJSONReader(std::unique_ptr<std::istream> inputStream, geopop::GeoGrid<EpiOutput>* epiOutput)
    : EpiOutputReader(move(inputStream), epiOutput)
{
}

void EpiOutputJSONReader::Read() {
    auto &geoGrid = *m_epiOutput;
    geoGrid.AddLocation(std::make_shared<geopop::Location<EpiOutput>>(0, 1, std::make_shared<EpiOutput>(),
                                                                      geopop::Coordinate(50.8503, 4.3517),
                                                                      "Brussel"));
    geoGrid[0]->getContent()->pop_count = 180000;
    geoGrid[0]->getContent()->epiOutput["Daycare"]["Total"][0] = 0;
    geoGrid[0]->getContent()->epiOutput["Daycare"]["Total"][50] = 0;
    geoGrid[0]->getContent()->epiOutput["Daycare"]["Total"][100] = 0;

    geoGrid.AddLocation(std::make_shared<geopop::Location<EpiOutput>>(3, 0, std::make_shared<EpiOutput>(),
                                                                      geopop::Coordinate(51.1683, 4.3943),
                                                                      "Wilrijk"));
    geoGrid[1]->getContent()->pop_count = 40943;

    geoGrid.AddLocation(std::make_shared<geopop::Location<EpiOutput>>(2, 2, std::make_shared<EpiOutput>(),
                                                                      geopop::Coordinate(50.9273, 4.4258),
                                                                      "Vilvoorde"));
    geoGrid[2]->getContent()->pop_count = 37964;

    geoGrid.AddLocation(std::make_shared<geopop::Location<EpiOutput>>(1, 0, std::make_shared<EpiOutput>(),
                                                                      geopop::Coordinate(51.2194, 4.4025),
                                                                      "Antwerpen"));
    geoGrid[3]->getContent()->pop_count = 520504;

    geoGrid.AddLocation(std::make_shared<geopop::Location<EpiOutput>>(4, 3, std::make_shared<EpiOutput>(),
                                                                      geopop::Coordinate(31.2304, 121.4737),
                                                                      "Shanghai"));
    geoGrid[4]->getContent()->pop_count = 24100000;
}

} // namespace visualization
