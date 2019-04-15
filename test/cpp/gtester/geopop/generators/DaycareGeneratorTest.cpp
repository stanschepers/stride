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

#include "geopop/generators/Generator.h"

#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "geopop/PoolParams.h"
#include "pop/Population.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

class DaycareGeneratorTest : public testing::Test {
public:
        DaycareGeneratorTest()
        : m_rn_man(RnInfo()), m_daycare_generator(m_rn_man), m_geogrid_config(), m_pop(Population::Create()),
          m_geo_grid(m_pop.get())
        {
        }

protected:
        RnMan                        m_rn_man;
        DaycareGenerator             m_daycare_generator;
        GeoGridConfig                m_geogrid_config;
        shared_ptr<Population>       m_pop;
        GeoGrid                      m_geo_grid;
};

// Check that generator can handle one Location.
TEST_F(DaycareGeneratorTest, OneLocationTest)
{
        m_geogrid_config.param.pop_size          = 10000;
        m_geogrid_config.info.popcount_daycare   = 2000;

        auto loc1    = make_shared<Location>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);
        m_geo_grid.AddLocation(loc1);

        m_daycare_generator.Apply(m_geo_grid, m_geogrid_config);

        const auto& poolsOfLoc1 = loc1->CRefPools(Id::Daycare);
        EXPECT_EQ(poolsOfLoc1.size(), 223 * PoolParams<Id::Daycare>::pools);
}

// Check that generator can handle empty GeoGrid.
TEST_F(DaycareGeneratorTest, ZeroLocationTest)
{
        m_geogrid_config.param.pop_size             = 10000;
        m_geogrid_config.info.popcount_daycare   = 2000;

        m_daycare_generator.Apply(m_geo_grid, m_geogrid_config);

        EXPECT_EQ(m_geo_grid.size(), 0);
}

// Check that generator can handle five Locations.
TEST_F(DaycareGeneratorTest, FiveLocationsTest)
{
        m_geogrid_config.param.pop_size         = 37542 * 100;
        m_geogrid_config.info.popcount_daycare   = 750840;

        auto loc1 = make_shared<Location>(1, 4, Coordinate(0, 0), "Antwerpen", 10150 * 100);
        auto loc2 = make_shared<Location>(1, 4, Coordinate(0, 0), "Vlaams-Brabant", 10040 * 100);
        auto loc3 = make_shared<Location>(1, 4, Coordinate(0, 0), "Henegouwen", 7460 * 100);
        auto loc4 = make_shared<Location>(1, 4, Coordinate(0, 0), "Limburg", 3269 * 100);
        auto loc5 = make_shared<Location>(1, 4, Coordinate(0, 0), "Luxemburg", 4123 * 100);

        m_geo_grid.AddLocation(loc1);
        m_geo_grid.AddLocation(loc2);
        m_geo_grid.AddLocation(loc3);
        m_geo_grid.AddLocation(loc4);
        m_geo_grid.AddLocation(loc5);

        for (const shared_ptr<Location>& loc : m_geo_grid) {
                loc->SetPopFraction(static_cast<double>(loc->GetPopCount()) /
                                    static_cast<double>(m_geogrid_config.param.pop_size));
        }

        m_daycare_generator.Apply(m_geo_grid, m_geogrid_config);

        vector<unsigned int> sizes{24254, 23844, 17652, 7778, 9899};
        for (size_t i = 0; i < sizes.size(); i++) {
                EXPECT_EQ(sizes[i] * PoolParams<Id::Daycare>::pools,
                          m_geo_grid[i]->CRefPools(Id::Daycare).size() );
        }
}

} // namespace
