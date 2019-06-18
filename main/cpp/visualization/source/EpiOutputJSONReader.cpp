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

#include <nlohmann/json.hpp>

#include "EpiOutputJSONReader.h"
#include "contact/AgeBrackets.h"
#include "disease/Health.h"
#include "geopop/Coordinate.h"
#include "geopop/Location.h"

using json = nlohmann::json;

namespace visualization {

EpiOutputJSONReader::EpiOutputJSONReader(std::unique_ptr<std::istream> inputStream,
                                         geopop::GeoGrid<EpiOutput>*   epiOutput)
    : EpiOutputReader(epiOutput), m_inputStream(move(inputStream))
{
}

void EpiOutputJSONReader::Read()
{
        json data;

        try {
                *(m_inputStream.get()) >> data;
                data["locations"];
        } catch (const json::exception& error) {
                throw std::runtime_error(
                    "An error occured while parsing JSON. Please make sure valid JSON is provided.");
        }

        auto& geoGrid = *m_epiOutput;

        for (unsigned int i = 0; i < data["locations"].size(); i++) {
                geoGrid.AddLocation(std::make_shared<geopop::Location<EpiOutput>>(
                    data["locations"][i]["id"], data["locations"][i]["province"], std::make_shared<EpiOutput>(),
                    geopop::Coordinate(data["locations"][i]["coordinate"][1], data["locations"][i]["coordinate"][0]),
                    data["locations"][i]["name"]));
                geoGrid[i]->GetContent()->pop_count = data["locations"][i]["pop_count"];
                for (const std::string& ageBracket : stride::ageBrackets) {
                        for (const std::string& healthStatus : stride::healthStatuses) {
                                for (unsigned int day : data["measured_days"]) {
                                        geoGrid[i]->GetContent()->epiOutput[ageBracket][healthStatus][day] =
                                            data["locations"][i]["epi-output"][ageBracket][healthStatus]
                                                [std::to_string(day)];
                                }
                        }
                }
        }
}

} // namespace visualization
