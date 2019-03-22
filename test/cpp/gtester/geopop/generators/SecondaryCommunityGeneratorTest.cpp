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

#include "geopop/generators/SecondaryCommunityGenerator.h"

#include "geopop/ContactCenter.h"
#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "pop/Population.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

class SecondaryCommunityGeneratorTest : public testing::Test {
public:
        SecondaryCommunityGeneratorTest()
                : m_rn_man(RnInfo()), m_secondary_community_generator(m_rn_man), m_geogrid_config(),
                  m_pop(Population::Create()), m_geo_grid(m_pop.get())
        {
        }

protected:
        RnMan                        m_rn_man;
        SecondaryCommunityGenerator  m_secondary_community_generator;
        GeoGridConfig                m_geogrid_config;
        shared_ptr<Population>       m_pop;
        GeoGrid                      m_geo_grid;
};

TEST_F(SecondaryCommunityGeneratorTest, OneLocationTest)
{
        m_geogrid_config.input.pop_size = 10000;
;
        auto loc1    = make_shared<Location>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);
        m_geo_grid.AddLocation(loc1);

        const auto& c1 = loc1->CRefCenters(Id::SecondaryCommunity);
        EXPECT_EQ(c1.size(), 0);

        const auto& p1 = loc1->CRefPools(Id::SecondaryCommunity);
        EXPECT_EQ(p1.size(), 0);

        unsigned int                scCounter{1U};
        m_secondary_community_generator.Apply(m_geo_grid, m_geogrid_config, scCounter);
        
        EXPECT_EQ(c1.size(), 5);
        EXPECT_EQ(p1.size(), 5 * m_geogrid_config.pools.pools_per_secondary_community);
}

TEST_F(SecondaryCommunityGeneratorTest, EqualLocationTest)
{
        m_geogrid_config.input.pop_size = 100 * 100 * 1000;

        for (int i = 0; i < 10; i++) {
                m_geo_grid.AddLocation(
                    make_shared<Location>(1, 4, Coordinate(0, 0), "Location " + to_string(i), 10 * 1000 * 1000));
        }

        unsigned int                scCounter{1U};
        m_secondary_community_generator.Apply(m_geo_grid, m_geogrid_config, scCounter);

        vector<unsigned int> expected{546, 495, 475, 500, 463, 533, 472, 539, 496, 481};
        for (int i = 0; i < 10; i++) {
                const auto& c2 = m_geo_grid[i]->RefCenters(Id::SecondaryCommunity);
                EXPECT_EQ(expected[i], c2.size());
        }
}

TEST_F(SecondaryCommunityGeneratorTest, ZeroLocationTest)
{
        m_geogrid_config.input.pop_size = 10000;

        unsigned int                scCounter{1U};
        m_secondary_community_generator.Apply(m_geo_grid, m_geogrid_config, scCounter);

        EXPECT_EQ(m_geo_grid.size(), 0);
}

TEST_F(SecondaryCommunityGeneratorTest, FiveLocationsTest)
{
        m_geogrid_config.input.pop_size             = 37542 * 100;
        m_geogrid_config.popInfo.popcount_k12school = 750840;

        auto loc1    = make_shared<Location>(1, 4, Coordinate(0, 0), "Antwerpen", 10150 * 100);
        auto loc2    = make_shared<Location>(1, 4, Coordinate(0, 0), "Vlaams-Brabant", 10040 * 100);
        auto loc3    = make_shared<Location>(1, 4, Coordinate(0, 0), "Henegouwen", 7460 * 100);
        auto loc4    = make_shared<Location>(1, 4, Coordinate(0, 0), "Limburg", 3269 * 100);
        auto loc5    = make_shared<Location>(1, 4, Coordinate(0, 0), "Luxemburg", 4123 * 100);

        m_geo_grid.AddLocation(loc1);
        m_geo_grid.AddLocation(loc2);
        m_geo_grid.AddLocation(loc3);
        m_geo_grid.AddLocation(loc4);
        m_geo_grid.AddLocation(loc5);

        unsigned int                scCounter{1U};
        m_secondary_community_generator.Apply(m_geo_grid, m_geogrid_config, scCounter);

        vector<int> expectedCount{553, 518, 410, 173, 224};
        for (int i = 0; i < 5; i++) {
                const auto& cc = m_geo_grid[i]->CRefCenters(Id::SecondaryCommunity);
                EXPECT_EQ(expectedCount[i], cc.size());

                const auto& cp = m_geo_grid[i]->CRefPools(Id::SecondaryCommunity);
                EXPECT_EQ(expectedCount[i] * m_geogrid_config.pools.pools_per_secondary_community, cp.size());
        }
}

} // namespace