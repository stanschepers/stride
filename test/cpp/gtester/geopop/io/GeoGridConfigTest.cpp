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
 *  Copyright 2019, Jan Broeckhove.
 */


#include "geopop/GeoGridConfigTest.h"
#include "geopop/Location.h"
#include "geopop/generators/CollegeGenerator.h"
#include "pop/Population.h"
#include "util/LogUtils.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>
#include <map>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

    class GeoGridConfigTest : public testing::Test
    {
    public:
        GeoGridConfigTest()
                : m_geogrid_config(), m_pop(Population::Create()),
                  m_geo_grid(m_pop->RefGeoGrid())
        {
        }

    protected:

        GeoGridConfig          m_geogrid_config;
        shared_ptr<Population> m_pop;
        GeoGrid&               m_geo_grid;
    };

TEST_F(GeoGridConfigTest, SetDataTest)
{
        EXPECT_TRUE(false);
}

TEST_F(GeoGridConfigTest, SetDataPoolsTest)
{
        EXPECT_TRUE(false);
}

TEST_F(GeoGridConfigTest, SetDataInputTest)
{
        EXPECT_TRUE(false);
}

TEST_F(GeoGridConfigTest, SetDataRefHHTest)
{
        EXPECT_TRUE(false);
}

TEST_F(GeoGridConfigTest, SetDataPopInfoTest)
{
        EXPECT_TRUE(false);
}

} // namespace
