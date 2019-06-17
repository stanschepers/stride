#include <utility>

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
 *  Copyright 2018, 2019Jan Broeckhove and Bistromatics group.
 */

#include "GeoGridHDF5Reader.h"

#include "geopop/GeoGrid.h"
#include "pop/Person.h"
#include "pop/Population.h"
#include "util/Exception.h"

#include <H5Cpp.h>
#include <iostream>
#include <map>

namespace geopop {

using namespace std;
using namespace stride::ContactType;
using namespace H5;
using namespace H5Utils;

GeoGridHDF5Reader::GeoGridHDF5Reader(std::string inputFilePath, stride::Population* pop)
    : GeoGridFileReader(std::move(inputFilePath), pop)
{
}

void GeoGridHDF5Reader::Read()
{
        try {
                /*
                 * Turn off the auto-printing when failure occurs so that we can
                 * handle the errors appropriately
                 */
                Exception::dontPrint();

                /// Open file
                H5File file(m_inputFilePath, H5F_ACC_RDONLY);

                auto& geoGrid = m_population->RefGeoGrid();

                /// Read people dataset
                DataSet people(file.openDataSet("People"));
                ReadPeople(people);

                /// Read locations group
                Group locations(file.openGroup("Locations"));
                ReadLocations(locations, geoGrid);

                AddCommutes(geoGrid);
                m_commutes.clear();
                m_people.clear();

        } catch (Exception& e) {
                throw stride::util::Exception("There was an error reading the GeoGrid from the file: " +
                                              e.getDetailMsg());
        }
}

void GeoGridHDF5Reader::ReadPeople(const DataSet& people)
{
        size_t size;
        ReadAttribute(size, "size", people);

        vector<H5Person> people_data(size);
        people.read(people_data.data(), GetCompoundType<H5Person>());

        if (size > 0) {
                for (auto person : people_data) {
                        ReadPerson(person);
                }
        }
}

void GeoGridHDF5Reader::ReadPerson(const H5Person& person)
{
        auto stridePerson = m_population->CreatePerson(
            person.id, person.age, person.Household, person.Daycare, person.PreSchool, person.K12School, person.College,
            person.Workplace, person.PrimaryCommunity, person.SecondaryCommunity);

        m_people[stridePerson->GetId()] = stridePerson;
}

void GeoGridHDF5Reader::ReadLocations(Group const& locations, GeoGrid<Epidemiologic>& grid)
{
        size_t size;
        ReadAttribute(size, "size", locations);

        if (size > 0) {
                for (size_t idx = 0; idx < size; ++idx) {
                        Group location(locations.openGroup("Location" + to_string(idx)));
                        ReadLocation(location, grid);
                }
        }
}

void GeoGridHDF5Reader::ReadLocation(const H5::Group& location, GeoGrid<Epidemiologic>& grid)
{
        /// Location
        unsigned int id, pop_count, province;
        double       latitude, longitude;
        string       name;

        ReadAttribute(id, "id", location);
        ReadAttribute(pop_count, "pop_count", location);
        ReadAttribute(province, "province", location);
        ReadAttribute(longitude, "longitude", location);
        ReadAttribute(latitude, "latitude", location);
        ReadAttribute(name, "name", location);

        auto locationPtr =
            make_shared<Location<Epidemiologic>>(id, province, Coordinate(longitude, latitude), name, pop_count);

        /// Contact Pools
        Group        contact_pools(location.openGroup("ContactPools"));
        unsigned int contact_pool_count;

        ReadAttribute(contact_pool_count, "size", contact_pools);

        if (contact_pool_count > 0) {
                for (size_t idx = 0; idx < contact_pool_count; ++idx) {
                        DataSet pool(contact_pools.openDataSet("Pool" + to_string(idx)));
                        ReadContactPool(pool, locationPtr);
                }
        }

        /// Commutes
        DataSet      commutes(location.openDataSet("commutes"));
        unsigned int commutes_count;

        ReadAttribute(commutes_count, "size", commutes);

        vector<H5Commute> commute_data(commutes_count);
        commutes.read(commute_data.data(), GetCompoundType<H5Commute>());

        if (commutes_count > 0) {
                for (auto c : commute_data) {
                        m_commutes.emplace_back(id, c.to, c.proportion);
                }
        }

        grid.AddLocation(locationPtr);
}

void GeoGridHDF5Reader::ReadContactPool(const H5::DataSet& pool, const shared_ptr<Location<Epidemiologic>>& location)
{
        static const map<string, Id> types = {{"K12School", Id::K12School},
                                              {"PrimaryCommunity", Id::PrimaryCommunity},
                                              {"SecondaryCommunity", Id::SecondaryCommunity},
                                              {"College", Id::College},
                                              {"Household", Id::Household},
                                              {"Daycare", Id::Daycare},
                                              {"PreSchool", Id::PreSchool},
                                              {"Workplace", Id::Workplace}};

        size_t size;
        string type;

        ReadAttribute(size, "size", pool);
        ReadAttribute(type, "type", pool);

        Id                   type_id = types.at(type);
        vector<H5PoolPerson> pool_data(size);
        pool.read(pool_data.data(), GetCompoundType<H5PoolPerson>());

        // Don't use the id of the ContactPool but the let the Population create an id
        auto contactPoolPtr = m_population->RefPoolSys().CreateContactPool(type_id);
        location->GetContent()->RefPools(type_id).emplace_back(contactPoolPtr);

        if (size > 0) {
                for (auto p : pool_data) {
                        const auto  person_id = p.id;
                        const auto& person    = m_people.at(person_id);
                        contactPoolPtr->AddMember(person);
                        // Update original pool id with new pool id used in the population
                        person->SetPoolId(type_id, contactPoolPtr->GetId());
                }
        }
}

} // namespace geopop
