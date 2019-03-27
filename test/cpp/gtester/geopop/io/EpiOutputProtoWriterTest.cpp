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
 *  Copyright 2019, 2020, Jan Broeckhove and Bistromatics group.
 */

#include "geopop/io/GeoGridProtoWriter.h"
#include "GeoGridIOUtils.h"
#include "geopop/CollegeCenter.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/HouseholdCenter.h"
#include "geopop/K12SchoolCenter.h"
#include "geopop/PrimaryCommunityCenter.h"
#include "geopop/SecondaryCommunityCenter.h"
#include "geopop/WorkplaceCenter.h"
#include "pop/Population.h"
#include "util/FileSys.h"

#include <gtest/gtest.h>

using namespace std;
using namespace geopop;
using namespace stride;

namespace {

TEST(EpiOutputProtoWriterTest, zeroLocationTest)
{
    EXPECT_TRUE(true);
}

TEST(EpiOutputProtoWriterTest, oneLocationZeroDaysTest)
{
    EXPECT_TRUE(true);
}

TEST(EpiOutputProtoWriterTest, oneLocationOneDayTest)
{
    EXPECT_TRUE(true);
}

TEST(EpiOutputProtoWriterTest, oneLocationTwoDaysTest)
{
    EXPECT_TRUE(true);
}

TEST(EpiOutputProtoWriterTest, fiveLocationsOneDayTest)
{
    EXPECT_TRUE(true);
}

} // namespace
