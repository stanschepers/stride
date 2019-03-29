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
 */

#include "HouseholdJSONReader.h"

#include <nlohmann/json.hpp>
#include <sstream>

namespace geopop {

using namespace std;
using namespace stride::util;
using json = nlohmann::json;

HouseholdJSONReader::HouseholdJSONReader(std::unique_ptr<std::istream> inputStream)
    : m_input_stream(std::move(inputStream))
{
}

void HouseholdJSONReader::SetReferenceHouseholds(unsigned int&                           ref_person_count,
                                                 std::vector<std::vector<unsigned int>>& ref_ages)
{
        json data;

        try {
                *(m_input_stream.get()) >> data;
                data["householdsList"];
        } catch (const json::exception& error) {
                m_logger->error("An error occured while parsing JSON.");
                throw runtime_error("An error occured while parsing JSON. Please make sure valid JSON is provided.");
        }

        unsigned int p_count = 0U;

        for (auto householdIt = data["householdsList"].begin(); householdIt != data["householdsList"].end();
             ++householdIt) {
                try {
                        const vector<unsigned int>& household = *householdIt;

                        p_count += household.size();
                        ref_ages.emplace_back(household);

                } catch (const json::type_error& error) {
                        const vector<string>& household = *householdIt;

                        vector<unsigned int> householdConverted;
                        householdConverted.reserve(household.size());

                        stringstream stream;
                        stream << "[";

                        for (const auto& age : household) {
                                householdConverted.push_back(static_cast<unsigned int>(stoi(age)));
                                stream << "\"" << age << "\",";
                        }

                        stream.seekp(-1, stream.cur);
                        stream << "]";

                        p_count += household.size();
                        ref_ages.emplace_back(householdConverted);

                        string conversionWarning =
                            "HouseholdJSONReader: STRING interpreted as UNSIGNED INT while reading " + stream.str();

                        m_logger->warn(conversionWarning);
                }
        }
        ref_person_count = p_count;
}

} // namespace geopop
