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
#include "pop/Population.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

class DaycareGeneratorTest : public testing::Test
{
public:
        DaycareGeneratorTest()
            : m_rn_man(RnInfo()), m_daycare_generator(m_rn_man), m_gg_config(), m_pop(Population::Create()),
              m_geo_grid(m_pop.get())
        {
        }

protected:
        RnMan                  m_rn_man;
        DaycareGenerator       m_daycare_generator;
        GeoGridConfig          m_gg_config;
        shared_ptr<Population> m_pop;
        GeoGrid<Epidemiologic> m_geo_grid;
        unsigned int           m_ppday = m_gg_config.pools[Id::Daycare];
};

// Check that generator can handle one Location.
TEST_F(DaycareGeneratorTest, OneLocationTest)
{
        m_gg_config.param.pop_size        = 10000;
        m_gg_config.info.popcount_daycare = 2000;

        auto loc1 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);
        m_geo_grid.AddLocation(loc1);

        m_daycare_generator.Apply(m_geo_grid, m_gg_config);

        const auto& poolsOfLoc1 = loc1->GetContent()->CRefPools(Id::Daycare);
        EXPECT_EQ(poolsOfLoc1.size(), 223 * m_ppday);
}

// Check that generator can handle one Location with a large young/old fraction.
TEST_F(DaycareGeneratorTest, OneLocationLargeYOFractionTest)
{
        m_gg_config.param.pop_size        = 10000;
        m_gg_config.info.popcount_daycare = 2000;

        auto loc1 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);
        loc1->GetContent()->SetYoungOldFraction(100.0);
        m_geo_grid.AddLocation(loc1);

        m_daycare_generator.Apply(m_geo_grid, m_gg_config);

        const auto& poolsOfLoc1 = loc1->GetContent()->CRefPools(Id::Daycare);
        EXPECT_EQ(poolsOfLoc1.size(), 223 * m_ppday);
}

// Check that generator can handle empty GeoGrid.
TEST_F(DaycareGeneratorTest, ZeroLocationTest)
{
        m_gg_config.param.pop_size        = 10000;
        m_gg_config.info.popcount_daycare = 2000;

        m_daycare_generator.Apply(m_geo_grid, m_gg_config);

        EXPECT_EQ(m_geo_grid.size(), 0);
}

// Check that generator can handle five Locations.
TEST_F(DaycareGeneratorTest, FiveLocationsTest)
{
        m_gg_config.param.pop_size        = 37542 * 100;
        m_gg_config.info.popcount_daycare = 750840;

        auto loc1 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Antwerpen", 10150 * 100);
        auto loc2 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Vlaams-Brabant", 10040 * 100);
        auto loc3 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Henegouwen", 7460 * 100);
        auto loc4 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Limburg", 3269 * 100);
        auto loc5 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Luxemburg", 4123 * 100);

        m_geo_grid.AddLocation(loc1);
        m_geo_grid.AddLocation(loc2);
        m_geo_grid.AddLocation(loc3);
        m_geo_grid.AddLocation(loc4);
        m_geo_grid.AddLocation(loc5);

        for (const shared_ptr<Location<Epidemiologic>>& loc : m_geo_grid) {
                loc->GetContent()->SetPopFraction(static_cast<double>(loc->GetContent()->GetPopCount()) /
                                                  static_cast<double>(m_gg_config.param.pop_size));
        }

        m_daycare_generator.Apply(m_geo_grid, m_gg_config);

        vector<unsigned int> sizes{24254, 23844, 17652, 7778, 9899};
        for (size_t i = 0; i < sizes.size(); i++) {
                EXPECT_EQ(sizes[i] * m_ppday, m_geo_grid[i]->GetContent()->CRefPools(Id::Daycare).size());
        }
}

// Check that a larger or smaller y/o fraction lets to respectively more and less instances in that city.
TEST_F(DaycareGeneratorTest, FiveLocationsYOFractionTest)
{
        m_gg_config.param.pop_size        = 10000;
        m_gg_config.info.popcount_daycare = 1000;

        auto loc1 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Antwerpen", 500);
        auto loc2 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Vlaams-Brabant", 200);
        auto loc3 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Henegouwen", 100);
        auto loc4 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Limburg", 150);
        auto loc5 = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Luxemburg", 50);

        m_geo_grid.AddLocation(loc1);
        m_geo_grid.AddLocation(loc2);
        m_geo_grid.AddLocation(loc3);
        m_geo_grid.AddLocation(loc4);
        m_geo_grid.AddLocation(loc5);

        vector<double> fractions{2.0, 1.0, 1.75, 0.75, 0.5};
        unsigned int   i = 0U;
        for (const shared_ptr<Location<Epidemiologic>>& loc : m_geo_grid) {
                loc->GetContent()->SetPopFraction(static_cast<double>(loc->GetContent()->GetPopCount()) /
                                                  static_cast<double>(m_gg_config.param.pop_size));
                loc->GetContent()->SetYoungOldFraction(fractions[i]);
                i++;
        }

        m_daycare_generator.Apply(m_geo_grid, m_gg_config);

        vector<unsigned int> normalSizes{54, 28, 9, 16, 5};
        for (size_t i = 0; i < normalSizes.size(); i++) {
                if (fractions[i] <= 1.0) {
                        EXPECT_GT(normalSizes[i] * m_ppday, m_geo_grid[i]->GetContent()->CRefPools(Id::Daycare).size());
                } else {
                        EXPECT_LT(normalSizes[i] * m_ppday, m_geo_grid[i]->GetContent()->CRefPools(Id::Daycare).size());
                }
        }
}

} // namespace
