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

#include "EpiOutputJSONWriter.h"

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
using json = nlohmann::json;

EpiOutputJSONWriter::EpiOutputJSONWriter(const std::string& filename) : m_output(), m_fstream() {
        if (!filename.empty()){
            m_fstream = std::ofstream(filename);
        }
        m_output["measured_days"] = {};
}

void EpiOutputJSONWriter::Write()
{
        this->Write(m_fstream);
        m_fstream.close();
}

void EpiOutputJSONWriter::Write(std::ostream& stream) {
        stream << setw(4) << m_output;
}

void EpiOutputJSONWriter::Update(GeoGrid<Epidemiologic>& geoGrid, unsigned int day)
{
        m_output["measured_days"].push_back(day);
        if (day == 0){
                for (unsigned int i = 0; i < geoGrid.size(); i++) {
                        const auto loc = geoGrid[i];
                        m_output["locations"][i]["id"] = loc->GetID();
                        m_output["locations"][i]["name"] = loc->GetName();
                        m_output["locations"][i]["province"] = loc->GetProvince();
                        m_output["locations"][i]["coordinate"][0] = loc->get<0>();
                        m_output["locations"][i]["coordinate"][1] = loc->get<1>();
                        m_output["locations"][i]["pop_count"] = loc->GetContent()->GetPopCount();
                }
        }
        for (unsigned int i = 0; i < geoGrid.size(); i++) {
                auto epiOutput = geoGrid[i]->GetContent()->GenerateEpiOutput();
                for (const std::string& ageBracket: stride::ageBrackets){
                        for (const std::string& healthStatus: stride::healthStatuses){
                                m_output["locations"][i]["epi-output"][ageBracket][healthStatus][std::to_string(day)] = epiOutput[ageBracket][healthStatus];
                        }
                }
        }
}

} // namespace geopop
