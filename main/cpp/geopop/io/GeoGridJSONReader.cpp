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
 *  Copyright 2018, 2019, Jan Broeckhove and Bistromatics group.
 */

#include "GeoGridJSONReader.h"
#include "util/LogUtils.h"

#include "geopop/GeoGrid.h"
#include "pop/Population.h"
#include "util/Exception.h"

#include <boost/lexical_cast.hpp>

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;
using json = nlohmann::json;

GeoGridJSONReader::GeoGridJSONReader(unique_ptr<istream> inputStream, Population* pop)
    : GeoGridReader(move(inputStream), pop)
{
}

void GeoGridJSONReader::Read()
{
        json data;

        try {
                *(m_inputStream.get()) >> data;
                data["locations"];
                data["persons"];
        } catch (const json::exception& error) {
                throw runtime_error("An error occured while parsing JSON. Please make sure valid JSON is provided.");
        }

        auto& geoGrid   = m_population->RefGeoGrid();
        auto& locations = data["locations"];
        auto& people    = data["persons"];

        for (const auto& person : people) {
                auto stridePerson               = ParsePerson(person);
                m_people[stridePerson->GetId()] = stridePerson;
        }

        for (const auto& location : locations) {
                auto strideLocation = ParseLocation(location);
                geoGrid.AddLocation(move(strideLocation));
        }

        AddCommutes(geoGrid);
        m_commutes.clear();
        m_people.clear();
}

stride::Person* GeoGridJSONReader::ParsePerson(const nlohmann::json& person)
{
        const auto id   = boost::lexical_cast<unsigned int>(person["id"]);
        const auto age  = boost::lexical_cast<unsigned int>(person["age"]);
        const auto hhId = boost::lexical_cast<unsigned int>(person["Household"]);
        const auto dcId = boost::lexical_cast<unsigned int>(person["Daycare"]);
        const auto psId = boost::lexical_cast<unsigned int>(person["PreSchool"]);
        const auto ksId = boost::lexical_cast<unsigned int>(person["K12School"]);
        const auto coId = boost::lexical_cast<unsigned int>(person["College"]);
        const auto wpId = boost::lexical_cast<unsigned int>(person["Workplace"]);
        const auto pcId = boost::lexical_cast<unsigned int>(person["PrimaryCommunity"]);
        const auto scId = boost::lexical_cast<unsigned int>(person["SecondaryCommunity"]);

        return m_population->CreatePerson(id, age, hhId, dcId, psId, ksId, coId, wpId, pcId, scId);
}

std::shared_ptr<Location> GeoGridJSONReader::ParseLocation(const nlohmann::json& location)
{
        const auto id         = boost::lexical_cast<unsigned int>(location["id"]);
        const auto name       = location["name"];
        const auto province   = boost::lexical_cast<unsigned int>(location["province"]);
        const auto population = boost::lexical_cast<unsigned int>(location["population"]);
        const auto coordinate = ParseCoordinate(location["coordinate"]);

        auto locationPtr = make_shared<Location>(id, province, coordinate, name, population);

        for (const auto& contactPool : location["contactPools"]) {
                const string type = contactPool["class"];

                static const map<string, Id> types = {{"Daycare", Id::Daycare},
                                                      {"PreSchool", Id::PreSchool},
                                                      {"K12School", Id::K12School},
                                                      {"PrimaryCommunity", Id::PrimaryCommunity},
                                                      {"SecondaryCommunity", Id::SecondaryCommunity},
                                                      {"College", Id::College},
                                                      {"Household", Id::Household},
                                                      {"Workplace", Id::Workplace}};

                const auto typeId = types.at(type);

                for (const auto& pool : contactPool["pools"]) {
                        ParseContactPool(locationPtr, pool, typeId);
                }
        }

        for (const auto& commute : location["commutes"]) {
                const auto to         = boost::lexical_cast<unsigned int>(commute["to"]);
                const auto proportion = boost::lexical_cast<double>(commute["proportion"]);
                m_commutes.emplace_back(id, to, proportion);
        }

        return locationPtr;
}

Coordinate GeoGridJSONReader::ParseCoordinate(const nlohmann::json& coordinate)
{
        const auto longitude = boost::lexical_cast<double>(coordinate["longitude"]);
        const auto latitude  = boost::lexical_cast<double>(coordinate["latitude"]);
        return {longitude, latitude};
}

void GeoGridJSONReader::ParseContactPool(const shared_ptr<Location>& location, const json& contactPool,
                                         ContactType::Id type)
{
        // Don't use the id of the ContactPool but the let the Population create an id
        auto contactPoolPtr = m_population->RefPoolSys().CreateContactPool(type);
        location->RefPools(type).emplace_back(contactPoolPtr);

        for (const auto& id : contactPool["people"]) {
                const auto  person_id = boost::lexical_cast<unsigned int>(id);
                const auto& person    = m_people.at(person_id);
                contactPoolPtr->AddMember(person);
                // Update original pool id with new pool id used in the population
                person->SetPoolId(type, contactPoolPtr->GetId());
        }
}
} // namespace geopop
