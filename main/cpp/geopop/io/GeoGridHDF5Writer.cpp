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
        typedef struct commute_h5
        {
                unsigned int to;
                double       proportion;
        } commute_h5;

        /// Create compound datatype
        H5::CompType commute_t(sizeof(commute_h5));
        commute_t.insertMember("to", HOFFSET(commute_h5, to), H5::PredType::NATIVE_UINT);
        commute_t.insertMember("proportion", HOFFSET(commute_h5, proportion), H5::PredType::NATIVE_DOUBLE);

        /// Create dataset
        H5::DataSet dataset(locGroup.createDataSet("commutes", commute_t, CreateSpace(commutes.size())));
        WriteAttribute(commutes.size(), "size", dataset);

        vector<commute_h5> comms(commutes.size());
        for (unsigned long i = 0; i < commutes.size(); ++i) {
                comms[i].to         = commutes[i].first->GetID();
                comms[i].proportion = commutes[i].second;
        }

        dataset.write(comms.data(), commute_t);
}

void GeoGridHDF5Writer::WriteContactPool(const ContactPool& pool, Id type, H5::Group& cpGroup)
{

        typedef struct pool_person_h5
        {
                unsigned int id;
        } pool_person_h5;

        /// Create compound datatype
        H5::CompType pool_person_t(sizeof(pool_person_h5));
        pool_person_t.insertMember("id", HOFFSET(pool_person_h5, id), H5::PredType::NATIVE_UINT);

        /// Create dataset
        string      label = ToString(type) + to_string(pool.GetId());
        H5::DataSet dataset(cpGroup.createDataSet(label, pool_person_t, CreateSpace(pool.size())));
        WriteAttribute(pool.GetId(), "id", dataset);
        WriteAttribute(pool.size(), "size", dataset);
        WriteAttribute(ToString(type), "type", dataset);

        vector<pool_person_h5> people(pool.size());
        for (unsigned long i = 0; i < pool.size(); ++i) {
                people[i].id = pool[i]->GetId();
                m_persons_found.insert(pool[i]);
        }

        dataset.write(people.data(), pool_person_t);
}

void GeoGridHDF5Writer::WritePeople(H5::Group& rootGroup)
{
        typedef struct person_h5
        {
                unsigned int id;
                float        age;
                unsigned int household_id;
                unsigned int k12School_id;
                unsigned int college_id;
                unsigned int workplace_id;
                unsigned int primaryCommunity_id;
                unsigned int secondaryCommunity_id;
        } person_h5;

        /// Create compound datatype
        H5::CompType person_t(sizeof(person_h5));
        person_t.insertMember("id", HOFFSET(person_h5, id), H5::PredType::NATIVE_UINT);
        person_t.insertMember("age", HOFFSET(person_h5, age), H5::PredType::NATIVE_FLOAT);
        person_t.insertMember("household_id", HOFFSET(person_h5, household_id), H5::PredType::NATIVE_UINT);
        person_t.insertMember("k12School_id", HOFFSET(person_h5, k12School_id), H5::PredType::NATIVE_UINT);
        person_t.insertMember("college_id", HOFFSET(person_h5, college_id), H5::PredType::NATIVE_UINT);
        person_t.insertMember("workplace_id", HOFFSET(person_h5, workplace_id), H5::PredType::NATIVE_UINT);
        person_t.insertMember("primaryCommunity_id", HOFFSET(person_h5, primaryCommunity_id),
                              H5::PredType::NATIVE_UINT);
        person_t.insertMember("secondaryCommunity_id", HOFFSET(person_h5, secondaryCommunity_id),
                              H5::PredType::NATIVE_UINT);

        /// Create dataset
        H5::DataSet dataset(rootGroup.createDataSet("People", person_t, CreateSpace(m_persons_found.size())));
        WriteAttribute(m_persons_found.size(), "size", dataset);

        vector<person_h5> people(m_persons_found.size());
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

} // namespace geopop
