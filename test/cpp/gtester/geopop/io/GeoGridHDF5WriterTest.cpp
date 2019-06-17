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

#include "geopop/io/GeoGridHDF5Writer.h"
#include "GeoGridIOUtils.h"
#include "geopop/GeoGrid.h"
#include "geopop/io/H5Utils.h"
#include "pop/Population.h"
#include "util/FileSys.h"

#include <gtest/gtest.h>
#include <vector>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;
using namespace H5;
using namespace H5Utils;

namespace {

bool validateLocationAttributes(unsigned int id, unsigned int province, double latitude, double longitude,
                                const string& name, unsigned int pop_count, const Group& location)
{
        unsigned int read_id;
        unsigned int read_province;
        unsigned int read_pop_count;
        double       read_latitude;
        double       read_longitude;
        string       read_name;

        ReadAttribute(read_id, "id", location);
        ReadAttribute(read_province, "province", location);
        ReadAttribute(read_pop_count, "pop_count", location);
        ReadAttribute(read_latitude, "latitude", location);
        ReadAttribute(read_longitude, "longitude", location);
        ReadAttribute(read_name, "name", location);

        return read_id == id && read_province == province && read_pop_count == pop_count && read_latitude == latitude &&
               read_longitude == longitude && read_name == name;
}

TEST(GeoGridHDF5WriterTest, writeLocationsTest)
{
        auto pop     = Population::Create();
        auto geoGrid = GeoGrid<Epidemiologic>(pop.get());
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Bavikhove", 2500));
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(2, 3, Coordinate(0, 0), "Gent", 5000));
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(3, 2, Coordinate(0, 0), "Mons", 2500));

        GeoGridHDF5Writer writer(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/locationsTest.h5");
        writer.Write(geoGrid);

        H5File file(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/locationsTest.h5", H5F_ACC_RDONLY);

        Group loc0(file.openGroup("/Locations/Location0"));
        Group loc1(file.openGroup("/Locations/Location1"));
        Group loc2(file.openGroup("/Locations/Location2"));

        EXPECT_TRUE(validateLocationAttributes(1, 4, 0, 0, "Bavikhove", 2500, loc0));
        EXPECT_TRUE(validateLocationAttributes(2, 3, 0, 0, "Gent", 5000, loc1));
        EXPECT_TRUE(validateLocationAttributes(3, 2, 0, 0, "Mons", 2500, loc2));
}

TEST(GeoGridHDF5WriterTest, writePeopleTest)
{
        auto   pop        = Population::Create();
        auto   geoGridPtr = GetPopulatedGeoGrid(pop.get());
        Person p1         = pop->at(0);

        GeoGridHDF5Writer writer(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/peopleTest.h5");
        writer.Write(*geoGridPtr);

        H5File file(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/peopleTest.h5", H5F_ACC_RDONLY);

        {
                DataSet          people(file.openDataSet("/People"));
                vector<H5Person> data(1);
                people.read(data.data(), GetCompoundType<H5Person>());
                H5Person p2 = data[0];

                EXPECT_EQ(p1.GetId(), p2.id);
                EXPECT_EQ(p1.GetAge(), p2.age);

                EXPECT_EQ(p1.GetPoolId(Id::Household), p2.Household);
                EXPECT_EQ(p1.GetPoolId(Id::K12School), p2.K12School);
                EXPECT_EQ(p1.GetPoolId(Id::College), p2.College);
                EXPECT_EQ(p1.GetPoolId(Id::Workplace), p2.Workplace);
                EXPECT_EQ(p1.GetPoolId(Id::PrimaryCommunity), p2.PrimaryCommunity);
                EXPECT_EQ(p1.GetPoolId(Id::SecondaryCommunity), p2.SecondaryCommunity);
        }
        {
                DataSet              HHpool(file.openDataSet("/Locations/Location0/ContactPools/Pool0"));
                vector<H5PoolPerson> data(1);
                HHpool.read(data.data(), GetCompoundType<H5PoolPerson>());
                H5PoolPerson p2 = data[0];
                EXPECT_EQ(p1.GetId(), p2.id);
        }
        {
                DataSet              K12pool(file.openDataSet("/Locations/Location0/ContactPools/Pool1"));
                vector<H5PoolPerson> data(1);
                K12pool.read(data.data(), GetCompoundType<H5PoolPerson>());
                H5PoolPerson p2 = data[0];
                EXPECT_EQ(p1.GetId(), p2.id);
        }
        {
                DataSet              Cpool(file.openDataSet("/Locations/Location0/ContactPools/Pool2"));
                vector<H5PoolPerson> data(1);
                Cpool.read(data.data(), GetCompoundType<H5PoolPerson>());
                H5PoolPerson p2 = data[0];
                EXPECT_EQ(p1.GetId(), p2.id);
        }
        {
                DataSet              Wpool(file.openDataSet("/Locations/Location0/ContactPools/Pool3"));
                vector<H5PoolPerson> data(1);
                Wpool.read(data.data(), GetCompoundType<H5PoolPerson>());
                H5PoolPerson p2 = data[0];
                EXPECT_EQ(p1.GetId(), p2.id);
        }
        {
                DataSet              PCpool(file.openDataSet("/Locations/Location0/ContactPools/Pool4"));
                vector<H5PoolPerson> data(1);
                PCpool.read(data.data(), GetCompoundType<H5PoolPerson>());
                H5PoolPerson p2 = data[0];
                EXPECT_EQ(p1.GetId(), p2.id);
        }
        {
                DataSet              SCpool(file.openDataSet("/Locations/Location0/ContactPools/Pool5"));
                vector<H5PoolPerson> data(1);
                SCpool.read(data.data(), GetCompoundType<H5PoolPerson>());
                H5PoolPerson p2 = data[0];
                EXPECT_EQ(p1.GetId(), p2.id);
        }
}

TEST(GeoGridHDF5WriterTest, writeCommutesTest)
{
        auto pop        = Population::Create();
        auto geoGridPtr = GetCommutesGeoGrid(pop.get());

        GeoGridHDF5Writer writer(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/commutesTest.h5");
        writer.Write(*geoGridPtr);

        H5File file(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/commutesTest.h5", H5F_ACC_RDONLY);

        {
                DataSet           HHpool(file.openDataSet("/Locations/Location0/commutes"));
                vector<H5Commute> data(2);
                HHpool.read(data.data(), GetCompoundType<H5Commute>());

                EXPECT_EQ(2, data[0].to);
                EXPECT_EQ(0.5, data[0].proportion);
                EXPECT_EQ(3, data[1].to);
                EXPECT_EQ(0.25, data[1].proportion);
        }
        {
                DataSet           HHpool(file.openDataSet("/Locations/Location1/commutes"));
                vector<H5Commute> data(2);
                HHpool.read(data.data(), GetCompoundType<H5Commute>());

                EXPECT_EQ(1, data[0].to);
                EXPECT_EQ(0.75, data[0].proportion);
                EXPECT_EQ(3, data[1].to);
                EXPECT_EQ(0.5, data[1].proportion);
        }
        {
                DataSet HHpool(file.openDataSet("/Locations/Location2/commutes"));
                size_t  commute_count;
                ReadAttribute(commute_count, "size", HHpool);
                EXPECT_EQ(0, commute_count);
        }
}

TEST(GeoGridHDF5WriterTest, writeContactPoolTest)
{
        auto pop     = Population::Create();
        auto geoGrid = GeoGrid<Epidemiologic>(pop.get());

        auto location = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Bavikhove", 2500);
        for (auto type : IdList) {
                auto contactPoolPtr = pop->RefPoolSys().CreateContactPool(type);
                location->GetContent()->RefPools(type).emplace_back(contactPoolPtr);
        }

        geoGrid.AddLocation(location);

        GeoGridHDF5Writer writer(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/contactPoolsTest.h5");
        writer.Write(geoGrid);

        H5File file(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/contactPoolsTest.h5", H5F_ACC_RDONLY);

        for (Id tid : IdList) {
                DataSet pool(file.openDataSet("/Locations/Location0/ContactPools/Pool" + to_string(ToSizeT(tid))));

                string type;
                ReadAttribute(type, "type", pool);
                EXPECT_EQ(ToString(tid), type);

                unsigned int id;
                ReadAttribute(id, "id", pool);
                EXPECT_EQ(1, id);

                size_t size;
                ReadAttribute(size, "size", pool);
                EXPECT_EQ(0, size);
        }
}

} // namespace
