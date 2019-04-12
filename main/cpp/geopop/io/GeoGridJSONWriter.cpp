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

#include "GeoGridJSONWriter.h"

#include "contact/ContactPool.h"
#include "geopop/GeoGrid.h"
#include "pop/Person.h"

#include <iomanip>
#include <iostream>

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;
using json = nlohmann::json;

GeoGridJSONWriter::GeoGridJSONWriter() : m_persons_found() {}

void GeoGridJSONWriter::Write(GeoGrid& geoGrid, ostream& stream)
{
        json output;

        output["locations"] = json::array();
        for (const auto& location : geoGrid) {
                output["locations"].push_back(WriteLocation(*location));
        }

        output["persons"] = json::array();
        for (const auto& person : m_persons_found) {
                output["persons"].push_back(WritePerson(person));
        }

        stream << setw(4) << output;
}

nlohmann::json GeoGridJSONWriter::WriteLocation(const Location& location)
{
        json locationJSON;

        locationJSON["id"]           = location.GetID();
        locationJSON["name"]         = location.GetName();
        locationJSON["province"]     = location.GetProvince();
        locationJSON["population"]   = location.GetPopCount();
        locationJSON["coordinate"]   = WriteCoordinate(location.GetCoordinate());
        locationJSON["contactPools"] = json::array();
        locationJSON["commutes"]     = json::array();

        for (const auto& type : IdList) {

                json contactPoolJSON;
                contactPoolJSON["class"] = ToString(type);
                contactPoolJSON["pools"] = json::array();

                for (const auto& pool : location.CRefPools(type)) {
                        contactPoolJSON["pools"].push_back(WriteContactPool(pool));
                }
                locationJSON["contactPools"].push_back(contactPoolJSON);
        }

        for (const auto& commute : location.CRefOutgoingCommutes()) {
                json commuteJSON;
                commuteJSON["to"]         = get<0>(commute)->GetID();
                commuteJSON["proportion"] = get<1>(commute);
                locationJSON["commutes"].push_back(commuteJSON);
        }

        return locationJSON;
}

nlohmann::json GeoGridJSONWriter::WriteCoordinate(const Coordinate& coordinate)
{
        json coordinateJSON;

        coordinateJSON["longitude"] = coordinate.get<0>();
        coordinateJSON["latitude"]  = coordinate.get<1>();

        return coordinateJSON;
}

nlohmann::json GeoGridJSONWriter::WriteContactPool(const ContactPool* contactPool)
{
        json poolJSON;

        poolJSON["id"]     = contactPool->GetId();
        poolJSON["people"] = json::array();

        for (const auto& person : contactPool->GetPool()) {
                m_persons_found.insert(person);
                poolJSON["people"].push_back(person->GetId());
        }

        return poolJSON;
}

nlohmann::json GeoGridJSONWriter::WritePerson(stride::Person* person)
{
        json personJSON;

        personJSON["id"]                 = person->GetId();
        personJSON["age"]                = (unsigned int)person->GetAge();
        personJSON["K12School"]          = person->GetPoolId(Id::K12School);
        personJSON["College"]            = person->GetPoolId(Id::College);
        personJSON["Household"]          = person->GetPoolId(Id::Household);
        personJSON["Workplace"]          = person->GetPoolId(Id::Workplace);
        personJSON["PrimaryCommunity"]   = person->GetPoolId(Id::PrimaryCommunity);
        personJSON["SecondaryCommunity"] = person->GetPoolId(Id::SecondaryCommunity);

        return personJSON;
}

} // namespace geopop
