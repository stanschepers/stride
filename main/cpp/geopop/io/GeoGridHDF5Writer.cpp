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
using namespace stride::util;
using namespace stride::ContactType;

GeoGridHDF5Writer::GeoGridHDF5Writer(string fileName) : GeoGridFileWriter(move(fileName)), m_persons_found() {}

void GeoGridHDF5Writer::Write(GeoGrid& geoGrid)
{
        try {
                // Turn off the auto-printing when failure occurs so that we can
                // handle the errors appropriately
                //                H5::Exception::dontPrint();

                H5::H5File file(m_file_name, H5F_ACC_TRUNC);

                H5::Group locations_group(file.createGroup("/Locations"));
                WriteAttribute(geoGrid.size(), "size", locations_group);

                for (const auto& location : geoGrid) {
                        WriteLocation(*location, locations_group);
                }

                WritePeople(file);

                m_persons_found.clear();

        } catch (const H5::Exception& e) {
                throw stride::util::Exception("There was an error writing the GeoGrid to the file: " +
                                              e.getDetailMsg());
        }
}

void GeoGridHDF5Writer::WriteLocation(Location& loc, H5::Group& locsGroup)
{
        /// Create group for location

        H5::Group locGroup(locsGroup.createGroup("Location" + to_string(loc.GetID())));
        /// Set location attributes
        WriteAttribute(loc.GetID(), "id", locGroup);
        WriteAttribute(loc.GetName(), "name", locGroup);
        WriteAttribute(loc.GetProvince(), "province", locGroup);
        WriteAttribute(loc.GetPopCount(), "pop_count", locGroup);
        WriteAttribute(loc.GetCoordinate().get<0>(), "longitude", locGroup);
        WriteAttribute(loc.GetCoordinate().get<1>(), "latitude", locGroup);

        auto commutes = loc.CRefOutgoingCommutes();
        WriteCommutes(commutes, locGroup);

        H5::Group    cpGroup(locGroup.createGroup("ContactPools"));
        unsigned int pool_count = 0;
        for (Id type : IdList) {
                for (auto pool : loc.RefPools(type)) {
                        ++pool_count;
                        WriteContactPool(*pool, type, cpGroup);
                }
        }
        WriteAttribute(pool_count, "size", cpGroup);
}

void GeoGridHDF5Writer::WriteCommutes(const vector<pair<Location*, double>>& commutes, H5::Group& locGroup)
{
        /// Create compound datatype
        H5::CompType commute_t = H5Utils::GetCompoundType<H5Utils::H5Commute>();

        /// Create dataset
        H5::DataSet dataset(locGroup.createDataSet("commutes", commute_t, CreateSpace(commutes.size())));
        WriteAttribute(commutes.size(), "size", dataset);

        vector<H5Utils::H5Commute> comms(commutes.size());
        for (unsigned long i = 0; i < commutes.size(); ++i) {
                comms[i].to         = commutes[i].first->GetID();
                comms[i].proportion = commutes[i].second;
        }

        dataset.write(comms.data(), commute_t);
}

void GeoGridHDF5Writer::WriteContactPool(const ContactPool& pool, Id type, H5::Group& cpGroup)
{

        H5::CompType pool_person_t = H5Utils::GetCompoundType<H5Utils::H5PoolPerson>();

        /// Create dataset
        string      label = ToString(type) + to_string(pool.GetId());
        H5::DataSet dataset(cpGroup.createDataSet(label, pool_person_t, CreateSpace(pool.size())));
        WriteAttribute(pool.GetId(), "id", dataset);
        WriteAttribute(pool.size(), "size", dataset);
        WriteAttribute(ToString(type), "type", dataset);

        vector<H5Utils::H5PoolPerson> people(pool.size());
        for (unsigned long i = 0; i < pool.size(); ++i) {
                people[i].id = pool[i]->GetId();
                m_persons_found.insert(pool[i]);
        }

        dataset.write(people.data(), pool_person_t);
}

void GeoGridHDF5Writer::WritePeople(H5::Group& rootGroup)
{

        /// Create compound datatype
        H5::CompType person_t = H5Utils::GetCompoundType<H5Utils::H5Person>();

        /// Create dataset
        H5::DataSet dataset(rootGroup.createDataSet("People", person_t, CreateSpace(m_persons_found.size())));
        WriteAttribute(m_persons_found.size(), "size", dataset);

        vector<H5Utils::H5Person> people(m_persons_found.size());
        int               y = 0;
        for (auto p : m_persons_found) {
                people[y].id                    = p->GetId();
                people[y].age                   = p->GetAge();
                people[y].household_id          = p->GetPoolId(Id::Household);
                people[y].k12School_id          = p->GetPoolId(Id::K12School);
                people[y].college_id            = p->GetPoolId(Id::College);
                people[y].workplace_id          = p->GetPoolId(Id::Workplace);
                people[y].primaryCommunity_id   = p->GetPoolId(Id::PrimaryCommunity);
                people[y].secondaryCommunity_id = p->GetPoolId(Id::SecondaryCommunity);
                y++;
        }

        dataset.write(people.data(), person_t);
}

template <typename T>
void GeoGridHDF5Writer::WriteAttribute(T value, const std::string& name, H5::H5Object& h5Object)
{
        hsize_t       one_dim[] = {1};
        H5::DataSpace atom(1, one_dim);
        auto          attribute(h5Object.createAttribute(name, HDF5Type(value), atom));
        attribute.write(HDF5Type(value), &value);
}

H5::DataSpace GeoGridHDF5Writer::CreateSpace(hsize_t size)
{
        hsize_t dim[] = {size};
        return H5::DataSpace(1, dim);
}

template <typename T>
auto GeoGridHDF5Writer::HDF5Type(const T&)
{
        throw stride::util::Exception("Tried to write unsupported type to HDF5.");
}

template <>
auto GeoGridHDF5Writer::HDF5Type(const int&)
{
        return H5::PredType::NATIVE_INT;
}

template <>
auto GeoGridHDF5Writer::HDF5Type(const unsigned int&)
{
        return H5::PredType::NATIVE_UINT;
}

template <>
auto GeoGridHDF5Writer::HDF5Type(const long&)
{
        return H5::PredType::NATIVE_LONG;
}

template <>
auto GeoGridHDF5Writer::HDF5Type(const unsigned long&)
{
        return H5::PredType::NATIVE_ULONG;
}

template <>
auto GeoGridHDF5Writer::HDF5Type(const float&)
{
        return H5::PredType::NATIVE_FLOAT;
}

template <>
auto GeoGridHDF5Writer::HDF5Type(const double&)
{
        return H5::PredType::NATIVE_DOUBLE;
}

template <>
auto GeoGridHDF5Writer::HDF5Type(const std::string& value)
{
        auto str_length = value.length();
        return H5::StrType(H5::PredType::C_S1, str_length + 1);
}

} // namespace geopop
