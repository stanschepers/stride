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
#include "geopop/io/GeoGridHDF5Reader.h"
#include "pop/Population.h"
#include "util/Exception.h"
#include "util/FileSys.h"

#include <gtest/gtest.h>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;
using namespace H5;
using namespace H5Utils;

namespace {

TEST(GeoGridHDF5ReaderTest, readLocationsTest)
{
        auto              pop = Population::Create();
        GeoGridHDF5Reader reader(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/locationsTest.h5", pop.get());
        reader.Read();

        auto&      geogrid = pop->CRefGeoGrid();
        Coordinate coordinate(0, 0);
        // Bavikhove
        {
                auto loc = geogrid[0];
                EXPECT_EQ(1, loc->GetID());
                EXPECT_EQ(2500, loc->GetPopCount());
                EXPECT_EQ(4, loc->GetProvince());
                EXPECT_EQ("Bavikhove", loc->GetName());
                EXPECT_EQ(coordinate.get<0>(), loc->GetCoordinate().get<0>());
                EXPECT_EQ(coordinate.get<1>(), loc->GetCoordinate().get<1>());
        }
        // Gent
        {
                auto loc = geogrid[1];
                EXPECT_EQ(2, loc->GetID());
                EXPECT_EQ(5000, loc->GetPopCount());
                EXPECT_EQ(3, loc->GetProvince());
                EXPECT_EQ("Gent", loc->GetName());
                EXPECT_EQ(coordinate.get<0>(), loc->GetCoordinate().get<0>());
                EXPECT_EQ(coordinate.get<1>(), loc->GetCoordinate().get<1>());
        }
        // Mons
        {
                auto loc = geogrid[2];
                EXPECT_EQ(3, loc->GetID());
                EXPECT_EQ(2500, loc->GetPopCount());
                EXPECT_EQ(2, loc->GetProvince());
                EXPECT_EQ("Mons", loc->GetName());
                EXPECT_EQ(coordinate.get<0>(), loc->GetCoordinate().get<0>());
                EXPECT_EQ(coordinate.get<1>(), loc->GetCoordinate().get<1>());
        }
}
TEST(GeoGridHDF5ReaderTest, readCommutesTest)
{
        auto              pop = Population::Create();
        GeoGridHDF5Reader reader(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/commutesTest.h5", pop.get());
        reader.Read();

        auto& geogrid = pop->CRefGeoGrid();
        // Bavikhove
        {
                auto  loc      = geogrid[0];
                auto& commutes = loc->CRefOutgoingCommutes();
                EXPECT_EQ(2, commutes[0].first->GetID());
                EXPECT_EQ(0.5, commutes[0].second);
                EXPECT_EQ(3, commutes[1].first->GetID());
                EXPECT_EQ(0.25, commutes[1].second);
        }
        // Gent
        {
                auto  loc      = geogrid[1];
                auto& commutes = loc->CRefOutgoingCommutes();
                EXPECT_EQ(1, commutes[0].first->GetID());
                EXPECT_EQ(0.75, commutes[0].second);
                EXPECT_EQ(3, commutes[1].first->GetID());
                EXPECT_EQ(0.5, commutes[1].second);
        }
        // Mons
        {
                auto  loc      = geogrid[2];
                auto& commutes = loc->CRefOutgoingCommutes();
                EXPECT_EQ(0, commutes.size());
        }
}
TEST(GeoGridHDF5ReaderTest, readContactPoolTest)
{
        auto              pop = Population::Create();
        GeoGridHDF5Reader reader(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/contactPoolsTest.h5",
                                 pop.get());
        reader.Read();

        auto& geogrid = pop->CRefGeoGrid();
        // Bavikhove
        auto loc = geogrid[0];
        for (Id type : IdList) {
                EXPECT_EQ(1, loc->CRefPools(type).size());
        }
}
TEST(GeoGridHDF5ReaderTest, readPeopleTest)
{
        auto              pop = Population::Create();
        GeoGridHDF5Reader reader(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/commutesTest.h5", pop.get());
        reader.Read();

        auto& geogrid = pop->CRefGeoGrid();
        auto& person  = pop->at(0);
        auto  loc     = geogrid[0];

        EXPECT_EQ(0, person.GetId());
        EXPECT_EQ(18.0, person.GetAge());
        for (Id type : IdList) {
                EXPECT_EQ(0, person.GetPoolId(type));
                EXPECT_EQ(0, loc->CRefPools(type).at(0)->GetId());
        }
}
TEST(GeoGridHDF5ReaderTest, emptyFileTest)
{
        auto pop = Population::Create();

        GeoGridHDF5Reader reader(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/emptyTest.h5", pop.get());
        EXPECT_THROW(reader.Read(), stride::util::Exception);
}

} // namespace
