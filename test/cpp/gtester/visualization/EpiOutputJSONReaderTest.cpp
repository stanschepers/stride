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

#include "geopop/io/GeoGridJSONReader.h"

#include "contact/ContactType.h"
#include "geopop/ContactCenter.h"
#include "geopop/GeoGrid.h"
#include "pop/Population.h"
#include "util/Exception.h"
#include "util/FileSys.h"

#include <fstream>
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

using boost::geometry::get;

namespace {

TEST(EpiOutputJSONWriterTest, zeroLocationTest)
{
    EXPECT_TRUE(false);
}

TEST(EpiOutputJSONWriterTest, oneLocationZeroDaysTest)
{
    EXPECT_TRUE(false);
}

TEST(EpiOutputJSONWriterTest, oneLocationOneDayTest)
{
    EXPECT_TRUE(false);
}

TEST(EpiOutputJSONWriterTest, oneLocationTwoDaysTest)
{
    EXPECT_TRUE(false);
}

TEST(EpiOutputJSONWriterTest, fiveLocationsOneDayTest)
{
    EXPECT_TRUE(false);
}

TEST(EpiOutputJSONWriterTest, emptyStreamTest)
{
    EXPECT_TRUE(false);
}

TEST(EpiOutputJSONWriterTest, invalidTypeTest)
{
    EXPECT_TRUE(false);
}

TEST(EpiOutputJSONWriterTest, invalidJSONTest)
{
    EXPECT_TRUE(false);
}

} // namespace
