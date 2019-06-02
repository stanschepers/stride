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

#include <fstream>
#include <memory>
#include <stdexcept>

#include "geopop/io/proto_pb/epioutput.pb.h"

#include "EpiOutputProtoReader.h"
#include "contact/AgeBrackets.h"
#include "disease/Health.h"
#include "geopop/Location.h"

namespace visualization {

EpiOutputProtoReader::EpiOutputProtoReader(std::unique_ptr<std::istream> inputStream,
                                           geopop::GeoGrid<EpiOutput>*   epiOutput)
    : EpiOutputReader(move(inputStream), epiOutput)
{
}

void EpiOutputProtoReader::Read()
{
        proto::EpiOutput protoEpiOutput;
        if (!protoEpiOutput.ParseFromIstream(m_inputStream.get())) {
                throw std::runtime_error("Failed to parse Proto file");
        }

        auto& geoGrid = *m_epiOutput;

        for (int l = 0; l < protoEpiOutput.days_size(); l++) {
                const auto& protoDay = protoEpiOutput.days(l);
                for (int i = 0; i < protoDay.locations_size(); i++) {
                        const auto& protoLoc  = protoDay.locations(i);
                        const auto& protoCoor = protoLoc.coordinate();
                        if(protoDay.day() == 0) {
                                geoGrid.AddLocation(std::make_shared<geopop::Location<EpiOutput>>(
                                    protoLoc.id(), protoLoc.province(), std::make_shared<EpiOutput>(),
                                    geopop::Coordinate(protoCoor.longitude(), protoCoor.latitude()), protoLoc.name()));
                        }
                        geoGrid[i]->GetContent()->pop_count = protoLoc.population();
                        for (int j = 0; j < protoLoc.agebrackets_size(); j++) {
                                const auto& protoAgeBracket = protoLoc.agebrackets(j);
                                for (int k = 0; k < protoAgeBracket.healthstatuses_size(); k++) {
                                        const auto& protoHealthStatus = protoAgeBracket.healthstatuses(k);
                                                geoGrid[i]->GetContent()->epiOutput[protoAgeBracket.type()][protoHealthStatus.type()][protoDay.day()] = protoHealthStatus.percentage();
                                }
                        }
                }
        }
}

} // namespace visualization
