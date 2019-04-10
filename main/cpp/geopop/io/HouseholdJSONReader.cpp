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

#include <boost/lexical_cast.hpp>

namespace geopop {

using namespace std;
using namespace stride::util;
using json = nlohmann::json;

HouseholdJSONReader::HouseholdJSONReader(std::unique_ptr<std::istream> inputStream)
    : m_input_stream(std::move(inputStream))
{
}

unsigned int HouseholdJSONReader::parseAge(const nlohmann::json& age) const
{
        if (age.is_string()) {
                string conversionWarning =
                    "HouseholdJSONReader: STRING interpreted as UNSIGNED INT while reading " + age.dump();

                cerr << conversionWarning << endl;
                m_logger->warn(conversionWarning);

                return boost::lexical_cast<unsigned int>(age.get<string>());
        } else {
                return age.get<unsigned int>();
        }
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

        for (const json& household : data["householdsList"]) {

                vector<unsigned int> parsedHH;
                parsedHH.reserve(household.size());

                for (const auto& age : household) {
                        parsedHH.push_back(parseAge(age));
                }

                p_count += household.size();
                ref_ages.emplace_back(parsedHH);
                
        }
        ref_person_count = p_count;
}

} // namespace geopop
