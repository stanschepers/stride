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
            unsigned int p_count = 0U;

            /*
             * std::vector<unsigned int> instead of auto to eliminate ambiguity when using GCC8 compiler
             * // NOLINT to suppress Clion (clang-tidy) code inspection
             */
            for (const std::vector<unsigned int>& household : data["householdsList"]) { // NOLINT
                p_count += household.size();
                ref_ages.emplace_back(household);
            }
            ref_person_count = p_count;
        } catch (const json::parse_error &error) {
            throw runtime_error("An error occured while parsing JSON. Please make sure valid JSON is provided.");
        } catch (const json::type_error &error) {
            throw runtime_error("Incorrect type encoutered while parsing households.");
        }
}

} // namespace geopop
