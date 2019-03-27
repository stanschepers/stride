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

#include "geopop/io/HouseholdJSONReader.h"
#include "geopop/GeoGridConfig.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace std;
using namespace geopop;
using namespace stride;

namespace {

shared_ptr<GeoGridConfig> readHouseholdsAndCreateGeoGridConfig(string jsonString)
{
        shared_ptr<GeoGridConfig> ggPtr    = make_shared<GeoGridConfig>();
        auto                      instream = make_unique<istringstream>(jsonString);
        HouseholdJSONReader       reader(move(instream));

        reader.SetReferenceHouseholds(ggPtr->refHH.person_count, ggPtr->refHH.ages);

        return ggPtr;
}

TEST(HouseholdJSONReader, emptyFileTest)
{
        string jsonString = R"()";

        EXPECT_THROW(readHouseholdsAndCreateGeoGridConfig(jsonString), runtime_error);
}
TEST(HouseholdJSONReader, noHouseholdListTest)
{
        string jsonString1 = R"({"householdsName":"TestHousehold","householdsList":[]})";
        string jsonString2 = R"({"householdsName":"TestHousehold"})";

        shared_ptr<GeoGridConfig> geoGridPtr1 = readHouseholdsAndCreateGeoGridConfig(jsonString1);
        shared_ptr<GeoGridConfig> geoGridPtr2 = readHouseholdsAndCreateGeoGridConfig(jsonString2);

        EXPECT_EQ(geoGridPtr1->refHH.person_count, 0U);
        EXPECT_EQ(geoGridPtr2->refHH.person_count, 0U);

        const vector<vector<unsigned int>>& HHages1 = geoGridPtr1->refHH.ages;
        const vector<vector<unsigned int>>& HHages2 = geoGridPtr2->refHH.ages;

        EXPECT_EQ(HHages1.size(), 0U);
        EXPECT_EQ(HHages2.size(), 0U);
}
TEST(HouseholdJSONReader, singleHouseholdTest)
{
        string jsonString = R"({"householdsName":"TestHousehold","householdsList":[[1,2,3,4,5,6,7,8,9,10]]})";

        shared_ptr<GeoGridConfig> geoGridPtr = readHouseholdsAndCreateGeoGridConfig(jsonString);

        EXPECT_EQ(geoGridPtr->refHH.person_count, 10U);

        const vector<vector<unsigned int>>& HHages = geoGridPtr->refHH.ages;

        EXPECT_EQ(HHages.size(), 1U);

        EXPECT_EQ(HHages[0].size(), 10U);

        EXPECT_EQ(HHages[0][0], 1U);
        EXPECT_EQ(HHages[0][1], 2U);
        EXPECT_EQ(HHages[0][2], 3U);
        EXPECT_EQ(HHages[0][3], 4U);
        EXPECT_EQ(HHages[0][4], 5U);
        EXPECT_EQ(HHages[0][5], 6U);
        EXPECT_EQ(HHages[0][6], 7U);
        EXPECT_EQ(HHages[0][7], 8U);
        EXPECT_EQ(HHages[0][8], 9U);
        EXPECT_EQ(HHages[0][9], 10U);
}
TEST(HouseholdJSONReader, multiHouseholdTest)
{
        string jsonString = R"({"householdsName":"TestHousehold","householdsList":[[1,2],[3,4],[5,6],[7,8],[9,10]]})";

        shared_ptr<GeoGridConfig> geoGridPtr = readHouseholdsAndCreateGeoGridConfig(jsonString);

        EXPECT_EQ(geoGridPtr->refHH.person_count, 10U);

        const vector<vector<unsigned int>>& HHages = geoGridPtr->refHH.ages;

        EXPECT_EQ(HHages.size(), 5U);

        EXPECT_EQ(HHages[0].size(), 2U);
        EXPECT_EQ(HHages[1].size(), 2U);
        EXPECT_EQ(HHages[2].size(), 2U);
        EXPECT_EQ(HHages[3].size(), 2U);
        EXPECT_EQ(HHages[4].size(), 2U);

        EXPECT_EQ(HHages[0][0], 1U);
        EXPECT_EQ(HHages[0][1], 2U);
        EXPECT_EQ(HHages[1][0], 3U);
        EXPECT_EQ(HHages[1][1], 4U);
        EXPECT_EQ(HHages[2][0], 5U);
        EXPECT_EQ(HHages[2][1], 6U);
        EXPECT_EQ(HHages[3][0], 7U);
        EXPECT_EQ(HHages[3][1], 8U);
        EXPECT_EQ(HHages[4][0], 9U);
        EXPECT_EQ(HHages[4][1], 10U);
}
TEST(HouseholdJSONReader, multiHouseholdVarSizeTest)
{
        string jsonString = R"({"householdsName":"TestHousehold","householdsList":[[1,2],[3,4,5,6],[7],[8,9,10]]})";

        shared_ptr<GeoGridConfig> geoGridPtr = readHouseholdsAndCreateGeoGridConfig(jsonString);

        EXPECT_EQ(geoGridPtr->refHH.person_count, 10U);

        const vector<vector<unsigned int>>& HHages = geoGridPtr->refHH.ages;

        EXPECT_EQ(HHages.size(), 4U);

        EXPECT_EQ(HHages[0].size(), 2U);
        EXPECT_EQ(HHages[1].size(), 4U);
        EXPECT_EQ(HHages[2].size(), 1U);
        EXPECT_EQ(HHages[3].size(), 3U);

        EXPECT_EQ(HHages[0][0], 1U);
        EXPECT_EQ(HHages[0][1], 2U);
        EXPECT_EQ(HHages[1][0], 3U);
        EXPECT_EQ(HHages[1][1], 4U);
        EXPECT_EQ(HHages[1][2], 5U);
        EXPECT_EQ(HHages[1][3], 6U);
        EXPECT_EQ(HHages[2][0], 7U);
        EXPECT_EQ(HHages[3][0], 8U);
        EXPECT_EQ(HHages[3][1], 9U);
        EXPECT_EQ(HHages[3][2], 10U);
}
TEST(HouseholdJSONReader, invalidJSONTest)
{
        string jsonString = R"("householdsName":"TestHousehold","householdsList"[1,2,3,4,5,6,7,8,9,10]]})";

        EXPECT_THROW(readHouseholdsAndCreateGeoGridConfig(jsonString), runtime_error);
}

} // namespace
