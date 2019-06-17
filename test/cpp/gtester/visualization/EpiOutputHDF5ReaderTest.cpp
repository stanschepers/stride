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

#include "visualization/source/EpiOutputHDF5Reader.h"
#include "geopop/GeoGrid.h"
#include "util/FileSys.h"

#include <gtest/gtest.h>
#include <vector>

namespace {

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::util;
using namespace visualization;

void getEpiOutputFromJSON(const string& h5File, geopop::GeoGrid<EpiOutput>* epiOutput)
{
        EpiOutputHDF5Reader epiOutputHDF5Reader(h5File, epiOutput);

        epiOutputHDF5Reader.Read();
}

TEST(EpiOutputHDF5ReaderTest, zeroLocationsTest)
{
        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        getEpiOutputFromJSON(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/zeroLocationsTest.h5",
                             &geoGrid);

        EXPECT_EQ(0, geoGrid.size());
}

TEST(EpiOutputHDF5ReaderTest, oneLocationZeroDaysTest)
{
        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        getEpiOutputFromJSON(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/zeroDaysTest.h5", &geoGrid);

        EXPECT_EQ(0, geoGrid.size());
}

TEST(EpiOutputHDF5ReaderTest, oneLocationOneDayTest)
{
        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        getEpiOutputFromJSON(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/oneLocationOneDayTest.h5",
                             &geoGrid);

        EXPECT_EQ(1, geoGrid.size());

        EXPECT_EQ(1, geoGrid[0]->GetID());
        EXPECT_EQ("test", geoGrid[0]->GetName());
        EXPECT_EQ(geopop::Coordinate(0,0).get<0>(), geoGrid[0]->GetCoordinate().get<0>());
        EXPECT_EQ(geopop::Coordinate(0,0).get<1>(), geoGrid[0]->GetCoordinate().get<1>());
        EXPECT_EQ(1, geoGrid[0]->GetProvince());
        EXPECT_EQ(0, geoGrid[0]->GetContent()->pop_count);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Susceptible"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Recovered"][0]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Susceptible"][0]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["K12School"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Recovered"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Immune"][0]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Infected"][0]);

        EXPECT_EQ(0.27999999999999997, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Infected"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Recovered"][0]);
        EXPECT_EQ(0.08, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Immune"][0]);

        EXPECT_EQ(0.12, geoGrid[0]->GetContent()->epiOutput["Senior"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Senior"]["Susceptible"][0]);
        EXPECT_EQ(0.02, geoGrid[0]->GetContent()->epiOutput["Senior"]["Immune"][0]);
}

TEST(EpiOutputHDF5ReaderTest, twoLocationsFiveDayTest)
{
        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        getEpiOutputFromJSON(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/twoLocationsFiveDaysTest.h5",
                             &geoGrid);

        EXPECT_EQ(2, geoGrid.size());

        EXPECT_EQ(0, geoGrid[0]->GetID());
        EXPECT_EQ("test0", geoGrid[0]->GetName());
        EXPECT_EQ(geopop::Coordinate(0,0).get<0>(), geoGrid[0]->get<0>());
        EXPECT_EQ(geopop::Coordinate(0,0).get<1>(), geoGrid[0]->get<1>());
        EXPECT_EQ(0, geoGrid[0]->GetProvince());
        EXPECT_EQ(0, geoGrid[0]->GetContent()->pop_count);

        EXPECT_EQ(1, geoGrid[1]->GetID());
        EXPECT_EQ("test1", geoGrid[1]->GetName());
        EXPECT_EQ(geopop::Coordinate(1.0,1.0).get<0>(), geoGrid[1]->get<0>());
        EXPECT_EQ(geopop::Coordinate(1.0,1.0).get<1>(), geoGrid[1]->get<1>());
        EXPECT_EQ(1, geoGrid[1]->GetProvince());
        EXPECT_EQ(100, geoGrid[1]->GetContent()->pop_count);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Susceptible"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Recovered"][0]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Susceptible"][0]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["K12School"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Recovered"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Immune"][0]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Infected"][0]);

        EXPECT_EQ(0.27999999999999997, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Infected"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Recovered"][0]);
        EXPECT_EQ(0.08, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Immune"][0]);

        EXPECT_EQ(0.12, geoGrid[0]->GetContent()->epiOutput["Senior"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Senior"]["Susceptible"][0]);
        EXPECT_EQ(0.02, geoGrid[0]->GetContent()->epiOutput["Senior"]["Immune"][0]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Susceptible"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Recovered"][10]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Susceptible"][10]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["K12School"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Recovered"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Immune"][10]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Infected"][10]);

        EXPECT_EQ(0.27999999999999997, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Infected"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Recovered"][10]);
        EXPECT_EQ(0.08, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Immune"][10]);

        EXPECT_EQ(0.12, geoGrid[0]->GetContent()->epiOutput["Senior"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Senior"]["Susceptible"][10]);
        EXPECT_EQ(0.02, geoGrid[0]->GetContent()->epiOutput["Senior"]["Immune"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Daycare"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Daycare"]["Susceptible"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Daycare"]["Recovered"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["PreSchool"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["PreSchool"]["Susceptible"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["K12School"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["K12School"]["Recovered"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["K12School"]["Immune"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["College"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["College"]["Infected"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Workplace"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Workplace"]["Infected"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Workplace"]["Recovered"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Workplace"]["Immune"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Senior"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Senior"]["Susceptible"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Senior"]["Immune"][10]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Susceptible"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Recovered"][50]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Susceptible"][50]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["K12School"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Recovered"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Immune"][50]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Infected"][50]);

        EXPECT_EQ(0.27999999999999997, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Infected"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Recovered"][50]);
        EXPECT_EQ(0.08, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Immune"][50]);

        EXPECT_EQ(0.12, geoGrid[0]->GetContent()->epiOutput["Senior"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Senior"]["Susceptible"][50]);
        EXPECT_EQ(0.02, geoGrid[0]->GetContent()->epiOutput["Senior"]["Immune"][50]);
}

} // namespace
