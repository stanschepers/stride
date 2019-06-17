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
 *  Copyright 2018, Jan Broeckhove and Bistromatics group.
 */

#include "GeoGridHDF5Writer.h"
#include "GeoGridHDF5Utils.h"

#include "contact/ContactPool.h"
#include "contact/ContactType.h"
#include "geopop/GeoGrid.h"
#include "pop/Person.h"
#include "util/SegmentedVector.h"

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;
using namespace H5;
using namespace H5Utils;

GeoGridHDF5Writer::GeoGridHDF5Writer(string fileName)
    : GeoGridFileWriter(move(fileName)), m_persons_found(), m_location_counter(), m_pool_counter()
{
}

void GeoGridHDF5Writer::Write(GeoGrid& geoGrid)
{
        try {
                // Turn off the auto-printing when failure occurs so that we can
                // handle the errors appropriately
                Exception::dontPrint();

                H5File file(m_file_name, H5F_ACC_TRUNC);

                Group locations_group(file.createGroup("/Locations"));
                WriteAttribute(geoGrid.size(), "size", locations_group);

                for (const auto& location : geoGrid) {
                        WriteLocation(*location, locations_group);
                }

                WritePeople(file);

                m_persons_found.clear();

        } catch (const Exception& e) {
                throw stride::util::Exception("There was an error writing the GeoGrid to the file: " +
                                              e.getDetailMsg());
        }
}

void GeoGridHDF5Writer::WriteLocation(Location& loc, Group& locsGroup)
{
        /// Create group for location

        Group locGroup(locsGroup.createGroup("Location" + to_string(m_location_counter++)));
        /// Set location attributes
        WriteAttribute(loc.GetID(), "id", locGroup);
        WriteAttribute(loc.GetName(), "name", locGroup);
        WriteAttribute(loc.GetProvince(), "province", locGroup);
        WriteAttribute(loc.GetPopCount(), "pop_count", locGroup);
        WriteAttribute(loc.GetCoordinate().get<0>(), "longitude", locGroup);
        WriteAttribute(loc.GetCoordinate().get<1>(), "latitude", locGroup);

        auto commutes = loc.CRefOutgoingCommutes();
        WriteCommutes(commutes, locGroup);

        Group cpGroup(locGroup.createGroup("ContactPools"));
        for (Id type : IdList) {
                for (auto pool : loc.RefPools(type)) {
                        WriteContactPool(*pool, type, cpGroup);
                }
        }
        WriteAttribute(m_pool_counter, "size", cpGroup);
        m_pool_counter = 0;
}

void GeoGridHDF5Writer::WriteCommutes(const vector<pair<Location*, double>>& commutes, Group& locGroup)
{
        /// Create compound datatype
        CompType commute_t = GetCompoundType<H5Commute>();

        /// Create dataset
        DataSet dataset(locGroup.createDataSet("commutes", commute_t, CreateSpace(commutes.size())));
        WriteAttribute(commutes.size(), "size", dataset);

        vector<H5Commute> comms(commutes.size());
        for (unsigned long i = 0; i < commutes.size(); ++i) {
                comms[i].to         = commutes[i].first->GetID();
                comms[i].proportion = commutes[i].second;
        }

        dataset.write(comms.data(), commute_t);
}

void GeoGridHDF5Writer::WriteContactPool(const ContactPool& pool, Id type, Group& cpGroup)
{

        CompType pool_person_t = GetCompoundType<H5PoolPerson>();

        /// Create dataset
        string  label = "Pool" + to_string(m_pool_counter++);
        DataSet dataset(cpGroup.createDataSet(label, pool_person_t, CreateSpace(pool.size())));
        WriteAttribute(pool.GetId(), "id", dataset);
        WriteAttribute(pool.size(), "size", dataset);
        WriteAttribute(ToString(type), "type", dataset);

        vector<H5PoolPerson> people(pool.size());
        for (unsigned long i = 0; i < pool.size(); ++i) {
                people[i].id = pool[i]->GetId();
                m_persons_found.insert(pool[i]);
        }

        dataset.write(people.data(), pool_person_t);
}

void GeoGridHDF5Writer::WritePeople(Group& rootGroup)
{

        /// Create compound datatype
        CompType person_t = GetCompoundType<H5Person>();

        /// Create dataset
        DataSet dataset(rootGroup.createDataSet("People", person_t, CreateSpace(m_persons_found.size())));
        WriteAttribute(m_persons_found.size(), "size", dataset);

        vector<H5Utils::H5Person> people(m_persons_found.size());
        int                       y = 0;
        for (auto p : m_persons_found) {
                people[y].id                 = p->GetId();
                people[y].age                = p->GetAge();
                people[y].Household          = p->GetPoolId(Id::Household);
                people[y].K12School          = p->GetPoolId(Id::K12School);
                people[y].College            = p->GetPoolId(Id::College);
                people[y].Workplace          = p->GetPoolId(Id::Workplace);
                people[y].PrimaryCommunity   = p->GetPoolId(Id::PrimaryCommunity);
                people[y].SecondaryCommunity = p->GetPoolId(Id::SecondaryCommunity);
                y++;
        }

        dataset.write(people.data(), person_t);
}

} // namespace geopop
