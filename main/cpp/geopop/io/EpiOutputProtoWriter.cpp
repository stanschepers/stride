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

#include "EpiOutputProtoWriter.h"

#include "geopop/GeoGrid.h"
#include "geopop/Location.h"
#include "disease/Health.h"
#include "contact/AgeBrackets.h"

#include <iostream>
#include <omp.h>

namespace geopop {

class Epidemiologic;

using namespace std;
using namespace stride;

EpiOutputProtoWriter::EpiOutputProtoWriter(const std::string& filename) : m_output(), m_fstream() {
        if (!filename.empty()){
                m_fstream = std::ofstream(filename);
        }
}

void EpiOutputProtoWriter::Write()
{
        this->Write(m_fstream);
        m_fstream.close();
}

void EpiOutputProtoWriter::Write(std::ostream& stream) {
        if (!m_output.SerializeToOstream(&stream)) {
                throw stride::util::Exception("There was an error writing the epi-output to the file.");
        }
        google::protobuf::ShutdownProtobufLibrary();
        stream.flush();
}

void EpiOutputProtoWriter::Update(GeoGrid<Epidemiologic>& geoGrid, unsigned int day)
{
        auto protoDay = m_output.add_days();
        protoDay->set_day(day);

        for (unsigned int i = 0; i < geoGrid.size(); i++) {
                const auto loc = geoGrid[i];

                auto protoLoc = protoDay->add_locations();
                protoLoc->set_id(loc->GetID());
                protoLoc->set_name(loc->GetName());
                protoLoc->set_province(loc->GetProvince());

                auto protoCoor = new proto::EpiOutput_Day_Location_Coordinate();
                protoCoor->set_latitude(loc->get<0>());
                protoCoor->set_longitude(loc->get<1>());

                protoLoc->set_allocated_coordinate(protoCoor);
                protoLoc->set_population(loc->GetContent()->GetPopCount());

                auto epiOutput = loc->GetContent()->GenerateEpiOutput();

                for (const std::string& ageBracket: stride::ageBrackets){
                        auto protoAgeBracket = protoLoc->add_agebrackets();
                        protoAgeBracket->set_type(ageBracket);
                        for (const std::string& healthStatus: stride::healthStatuses){
                                auto protoHealthStatus = protoAgeBracket->add_healthstatuses();
                                protoHealthStatus->set_type(healthStatus);
                                protoHealthStatus->set_percentage(epiOutput[ageBracket][healthStatus]);
                        }
                }
        }
}

} // namespace geopop
