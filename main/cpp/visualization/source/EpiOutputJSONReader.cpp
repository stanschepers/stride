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

#include "EpiOutputJSONReader.h"

#include <memory>
#include <iostream>

namespace visualization {

EpiOutputJSONReader::EpiOutputJSONReader(unique_ptr<istream> inputStream, Population* pop)
    : EpiOutputReader(move(inputStream), pop)
{
}

void EpiOutputJSONReader::Read()
{
        std::cout << "implementation?" << std::endl;
//        boost::property_tree::ptree root;
//        try {
//                boost::property_tree::read_json(*m_inputStream, root);
//        } catch (runtime_error&) {
//                throw Exception("Problem parsing JSON file, check whether empty or invalid JSON.");
//        }
//
//        auto& geoGrid = m_population->RefEpiOutput();
//        auto  people  = root.get_child("persons");
//
//        for (auto it = people.begin(); it != people.end(); it++) {
//                auto person               = ParsePerson(it->second.get_child(""));
//                m_people[person->GetId()] = person;
//        }
//        auto locations = root.get_child("locations");
//
//        for (auto it = locations.begin(); it != locations.end(); it++) {
//                shared_ptr<Location> loc;
//                loc = ParseLocation(it->second.get_child(""));
//                geoGrid.AddLocation(move(loc));
//        }
//
//        AddCommutes(geoGrid);
//        m_commutes.clear();
//        m_people.clear();
}

//shared_ptr<Location> EpiOutputJSONReader::ParseLocation(boost::property_tree::ptree& location)
//{
//        const auto id         = boost::lexical_cast<unsigned int>(location.get<string>("id"));
//        const auto name       = location.get<string>("name");
//        const auto province   = boost::lexical_cast<unsigned int>(location.get<string>("province"));
//        const auto population = boost::lexical_cast<unsigned int>(location.get<string>("population"));
//        const auto coordinate = ParseCoordinate(location.get_child("coordinate"));
//
//        auto result         = make_shared<Location>(id, province, coordinate, name, population);
//        auto contactCenters = location.get_child("contactCenters");
//
//        for (auto it = contactCenters.begin(); it != contactCenters.end(); it++) {
//                const auto center = ParseContactCenter(it->second.get_child(""));
//                result->AddCenter(center);
//        }
//
//        if (location.count("commutes")) {
//                boost::property_tree::ptree commutes = location.get_child("commutes");
//                for (auto it = commutes.begin(); it != commutes.end(); it++) {
//                        const auto to     = boost::lexical_cast<unsigned int>(it->first);
//                        const auto amount = boost::lexical_cast<double>(it->second.data());
//                        m_commutes.emplace_back(id, to, amount);
//                }
//        }
//
//        return result;
//}
//
//Coordinate EpiOutputJSONReader::ParseCoordinate(boost::property_tree::ptree& coordinate)
//{
//        const auto longitude = boost::lexical_cast<double>(coordinate.get<string>("longitude"));
//        const auto latitude  = boost::lexical_cast<double>(coordinate.get<string>("latitude"));
//        return {longitude, latitude};
//}
//
//shared_ptr<ContactCenter> EpiOutputJSONReader::ParseContactCenter(boost::property_tree::ptree& contactCenter)
//{
//        const auto type = contactCenter.get<string>("type");
//        const auto id   = boost::lexical_cast<unsigned int>(contactCenter.get<string>("id"));
//
//        ContactType::Id typeId;
//        if (type == ToString(Id::K12School)) {
//                typeId = Id::K12School;
//        } else if (type == ToString(Id::College)) {
//                typeId = Id::College;
//        } else if (type == ToString(Id::Household)) {
//                typeId = Id::Household;
//        } else if (type == ToString(Id::PrimaryCommunity)) {
//                typeId = Id::PrimaryCommunity;
//        } else if (type == ToString(Id::SecondaryCommunity)) {
//                typeId = Id::SecondaryCommunity;
//        } else if (type == ToString(Id::Workplace)) {
//                typeId = Id::Workplace;
//        } else {
//                throw Exception("No such ContactCenter type: " + type);
//        }
//        auto result       = make_shared<ContactCenter>(id, typeId);
//        auto contactPools = contactCenter.get_child("pools");
//
//        for (auto it = contactPools.begin(); it != contactPools.end(); it++) {
//                const auto pool = ParseContactPool(it->second.get_child(""), typeId);
//                result->RegisterPool(pool);
//        }
//
//        return result;
//}
//
//ContactPool* EpiOutputJSONReader::ParseContactPool(boost::property_tree::ptree& contactPool, ContactType::Id typeId)
//{
//        // Don't use the id of the ContactPool but the let the Population create an id.
//        auto result = m_population->RefPoolSys().CreateContactPool(typeId);
//        auto people = contactPool.get_child("people");
//
//        for (auto it = people.begin(); it != people.end(); it++) {
//                auto person_id = boost::lexical_cast<unsigned int>(it->second.get<string>(""));
//                if (m_people.count(person_id) == 0) {
//                        throw Exception("No such person: " + to_string(person_id));
//                }
//                result->AddMember(m_people[person_id]);
//        }
//
//        return result;
//}
//
//Person* EpiOutputJSONReader::ParsePerson(boost::property_tree::ptree& person)
//{
//        const auto id   = boost::lexical_cast<unsigned int>(person.get<string>("id"));
//        const auto age  = boost::lexical_cast<unsigned int>(person.get<string>("age"));
//        const auto hhId = boost::lexical_cast<unsigned int>(person.get<string>("Household"));
//        const auto ksId = boost::lexical_cast<unsigned int>(person.get<string>("K12School"));
//        const auto coId = boost::lexical_cast<unsigned int>(person.get<string>("College"));
//        const auto wpId = boost::lexical_cast<unsigned int>(person.get<string>("Workplace"));
//        const auto pcId = boost::lexical_cast<unsigned int>(person.get<string>("PrimaryCommunity"));
//        const auto scId = boost::lexical_cast<unsigned int>(person.get<string>("SecondaryCommunity"));
//
//        return m_population->CreatePerson(id, age, hhId, ksId, coId, wpId, pcId, scId);
//}

} // namespace visualization
