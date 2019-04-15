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

#include "geopop/populators/Populator.h"
#include "geopop/generators/Generator.h"

#include "MakeGeoGrid.h"
#include "contact/AgeBrackets.h"
#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
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

class DaycarePopulatorTest : public testing::Test
{
public:
        DaycarePopulatorTest()
            : m_rn_man(RnInfo()), m_daycare_populator(m_rn_man), m_geogrid_config(), m_pop(Population::Create()),
              m_geo_grid(m_pop->RefGeoGrid()), m_daycare_generator(m_rn_man)
        {
        }

protected:
        RnMan                  m_rn_man;
        DaycarePopulator       m_daycare_populator;
        GeoGridConfig          m_geogrid_config;
        shared_ptr<Population> m_pop;
        GeoGrid&               m_geo_grid;
        DaycareGenerator       m_daycare_generator;
};

// Check that populator can handle empty GeoGrid
TEST_F(DaycarePopulatorTest, NoPopulation)
{
        m_geo_grid.AddLocation(make_shared<Location>(0, 0, Coordinate(0.0, 0.0), "", 0));
        m_geo_grid.Finalize();

        EXPECT_NO_THROW(m_daycare_populator.Apply(m_geo_grid, m_geogrid_config));
}

// Check that populator can handle one Location
TEST_F(DaycarePopulatorTest, OneLocationTest)
{
        MakeGeoGrid(m_geogrid_config, 1, 300, 5, 100, 3, m_pop.get());
        m_geo_grid.Finalize();
        m_geogrid_config.param.participation_daycare = 1;
        m_daycare_populator.Apply(m_geo_grid, m_geogrid_config);

        ///{poolId, poolSize}
        map<int, int> usedCapacity{
            {1, 1},   {2, 2},   {3, 2},   {4, 1},   {5, 1},   {6, 0},   {7, 1},   {8, 1},   {9, 2},   {10, 1},
            {11, 1},  {12, 1},  {13, 0},  {14, 1},  {15, 0},  {16, 0},  {17, 0},  {18, 0},  {19, 0},  {20, 0},
            {21, 1},  {22, 0},  {23, 0},  {24, 1},  {25, 0},  {26, 1},  {27, 0},  {28, 1},  {29, 0},  {30, 0},
            {31, 0},  {32, 0},  {33, 2},  {34, 0},  {35, 0},  {36, 0},  {37, 0},  {38, 0},  {39, 0},  {40, 0},
            {41, 2},  {42, 0},  {43, 0},  {44, 0},  {45, 1},  {46, 0},  {47, 0},  {48, 0},  {49, 1},  {50, 0},
            {51, 0},  {52, 1},  {53, 0},  {54, 0},  {55, 0},  {56, 0},  {57, 1},  {58, 1},  {59, 0},  {60, 1},
            {61, 0},  {62, 1},  {63, 0},  {64, 0},  {65, 0},  {66, 0},  {67, 0},  {68, 0},  {69, 0},  {70, 1},
            {71, 0},  {72, 1},  {73, 1},  {74, 0},  {75, 0},  {76, 0},  {77, 0},  {78, 1},  {79, 1},  {80, 0},
            {81, 1},  {82, 2},  {83, 0},  {84, 0},  {85, 0},  {86, 1},  {87, 0},  {88, 1},  {89, 0},  {90, 0},
            {91, 0},  {92, 1},  {93, 0},  {94, 0},  {95, 1},  {96, 0},  {97, 0},  {98, 0},  {99, 1},  {100, 0},
            {101, 0}, {102, 0}, {103, 1}, {104, 0}, {105, 0}, {106, 1}, {107, 0}, {108, 0}, {109, 1}, {110, 0},
            {111, 1}, {112, 0}, {113, 1}, {114, 0}, {115, 0}, {116, 0}, {117, 0}, {118, 0}, {119, 0}, {120, 1},
            {121, 0}, {122, 1}, {123, 0}, {124, 0}, {125, 0}};

        auto  location = *m_geo_grid.begin();
        auto& dayPools = location->RefPools(Id::Daycare);

        ASSERT_EQ(dayPools.size(), 5 * PoolParams<Id::Daycare>::pools);
        for (auto& pool : dayPools) {
                EXPECT_EQ(usedCapacity[pool->GetId()], pool->size());
                for (Person* person : *pool) {
                        EXPECT_LE(person->GetAge(), AgeBrackets::Daycare::m_upper);
                        EXPECT_GE(person->GetAge(), AgeBrackets::Daycare::m_lower);
                }
        }

        ///{personId, poolId}
        map<int, int> persons{
            {0, 0},     {1, 0},     {2, 0},     {3, 0},     {4, 0},     {5, 0},    {6, 0},    {7, 0},    {8, 0},
            {9, 0},     {10, 0},    {11, 0},    {12, 0},    {13, 0},    {14, 0},   {15, 0},   {16, 0},   {17, 0},
            {18, 0},    {19, 0},    {20, 0},    {21, 0},    {22, 0},    {23, 0},   {24, 0},   {25, 5},   {26, 0},
            {27, 0},    {28, 0},    {29, 0},    {30, 0},    {31, 0},    {32, 0},   {33, 0},   {34, 0},   {35, 0},
            {36, 0},    {37, 0},    {38, 0},    {39, 0},    {40, 0},    {41, 0},   {42, 0},   {43, 0},   {44, 0},
            {45, 0},    {46, 0},    {47, 0},    {48, 0},    {49, 0},    {50, 0},   {51, 0},   {52, 0},   {53, 0},
            {54, 0},    {55, 0},    {56, 0},    {57, 0},    {58, 0},    {59, 0},   {60, 0},   {61, 0},   {62, 0},
            {63, 0},    {64, 0},    {65, 0},    {66, 0},    {67, 0},    {68, 0},   {69, 0},   {70, 0},   {71, 0},
            {72, 0},    {73, 0},    {74, 0},    {75, 0},    {76, 0},    {77, 0},   {78, 0},   {79, 0},   {80, 0},
            {81, 0},    {82, 0},    {83, 0},    {84, 0},    {85, 0},    {86, 0},   {87, 0},   {88, 0},   {89, 0},
            {90, 0},    {91, 0},    {92, 0},    {93, 0},    {94, 0},    {95, 0},   {96, 0},   {97, 0},   {98, 0},
            {99, 0},    {100, 0},   {101, 0},   {102, 0},   {103, 0},   {104, 0},  {105, 0},  {106, 0},  {107, 0},
            {108, 0},   {109, 0},   {110, 0},   {111, 0},   {112, 0},   {113, 0},  {114, 0},  {115, 3},  {116, 0},
            {117, 0},   {118, 0},   {119, 0},   {120, 0},   {121, 0},   {122, 0},  {123, 0},  {124, 0},  {125, 0},
            {126, 0},   {127, 0},   {128, 0},   {129, 0},   {130, 0},   {131, 0},  {132, 0},  {133, 0},  {134, 0},
            {135, 0},   {136, 0},   {137, 0},   {138, 0},   {139, 0},   {140, 0},  {141, 0},  {142, 4},  {143, 0},
            {144, 0},   {145, 2},   {146, 0},   {147, 0},   {148, 0},   {149, 0},  {150, 0},  {151, 0},  {152, 0},
            {153, 0},   {154, 0},   {155, 0},   {156, 0},   {157, 0},   {158, 0},  {159, 0},  {160, 0},  {161, 0},
            {162, 0},   {163, 0},   {164, 0},   {165, 0},   {166, 0},   {167, 0},  {168, 0},  {169, 0},  {170, 0},
            {171, 0},   {172, 0},   {173, 0},   {174, 0},   {175, 0},   {176, 0},  {177, 0},  {178, 0},  {179, 0},
            {180, 0},   {181, 0},   {182, 1},   {183, 0},   {184, 0},   {185, 0},  {186, 0},  {187, 0},  {188, 0},
            {189, 0},   {190, 0},   {191, 0},   {192, 0},   {193, 0},   {194, 0},  {195, 0},  {196, 0},  {197, 0},
            {198, 0},   {199, 0},   {200, 0},   {201, 2},   {202, 0},   {203, 0},  {204, 0},  {205, 0},  {206, 0},
            {207, 0},   {208, 0},   {209, 0},   {210, 0},   {211, 0},   {212, 0},  {213, 0},  {214, 0},  {215, 0},
            {216, 0},   {217, 0},   {218, 0},   {219, 0},   {220, 0},   {221, 0},  {222, 0},  {223, 0},  {224, 0},
            {225, 0},   {226, 0},   {227, 0},   {228, 0},   {229, 0},   {230, 0},  {231, 0},  {232, 0},  {233, 0},
            {234, 0},   {235, 0},   {236, 0},   {237, 0},   {238, 0},   {239, 0},  {240, 0},  {241, 0},  {242, 0},
            {243, 0},   {244, 0},   {245, 0},   {246, 0},   {247, 0},   {248, 0},  {249, 0},  {250, 0},  {251, 0},
            {252, 0},   {253, 0},   {254, 0},   {255, 0},   {256, 0},   {257, 0},  {258, 0},  {259, 0},  {260, 0},
            {261, 0},   {262, 0},   {263, 0},   {264, 0},   {265, 0},   {266, 0},  {267, 0},  {268, 0},  {269, 0},
            {270, 0},   {271, 0},   {272, 0},   {273, 0},   {274, 0},   {275, 0},  {276, 0},  {277, 0},  {278, 0},
            {279, 0},   {280, 0},   {281, 0},   {282, 0},   {283, 0},   {284, 0},  {285, 0},  {286, 0},  {287, 0},
            {288, 0},   {289, 0},   {290, 0},   {291, 0},   {292, 0},   {293, 3},  {294, 0},  {295, 0},  {296, 0},
            {297, 0},   {298, 0},   {299, 0}};

        for (const auto& person : *m_geo_grid.GetPopulation()) {
                EXPECT_EQ(persons[person.GetId()], person.GetPoolId(Id::Daycare));
        }
}

// Check that populator can handle two Locations
TEST_F(DaycarePopulatorTest, TwoLocationTest)
{
        MakeGeoGrid(m_geogrid_config, 3, 100, 3, 33, 3, m_pop.get());

        // Brasschaat and Schoten are close to each oter and will both have students from both.
        // Kortrijk will only have students going to Kortrijk.

        auto brasschaat = *m_geo_grid.begin();
        brasschaat->SetCoordinate(Coordinate(51.29227, 4.49419));
        auto schoten = *(m_geo_grid.begin() + 1);

        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));
        auto kortrijk = *(m_geo_grid.begin() + 2);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));

        m_geo_grid.Finalize();
        m_geogrid_config.param.participation_daycare = 1;
        m_daycare_populator.Apply(m_geo_grid, m_geogrid_config);

        auto& dayPools1 = brasschaat->RefPools(Id::Daycare);
        auto& dayPools2 = schoten->RefPools(Id::Daycare);
        auto& dayPools3 = kortrijk->RefPools(Id::Daycare);

        // Check number of pools corresponding to 3 K12Schools per location.
        EXPECT_EQ(dayPools1.size(), 3 * PoolParams<Id::Daycare>::pools);
        EXPECT_EQ(dayPools2.size(), 3 * PoolParams<Id::Daycare>::pools);
        EXPECT_EQ(dayPools3.size(), 3 * PoolParams<Id::Daycare>::pools);

        map<int, int> persons{
            {0, 0},     {1, 0},     {2, 0},     {3, 0},     {4, 0},     {5, 0},     {6, 0},     {7, 0},     {8, 0},
            {9, 0},     {10, 0},    {11, 0},    {12, 0},    {13, 0},    {14, 0},    {15, 0},    {16, 0},    {17, 0},
            {18, 0},    {19, 0},    {20, 0},    {21, 0},    {22, 0},    {23, 0},    {24, 0},    {25, 3},    {26, 0},
            {27, 0},    {28, 0},    {29, 0},    {30, 0},    {31, 0},    {32, 0},    {33, 0},    {34, 0},    {35, 0},
            {36, 0},    {37, 0},    {38, 0},    {39, 0},    {40, 0},    {41, 0},    {42, 0},    {43, 0},    {44, 0},
            {45, 0},    {46, 0},    {47, 0},    {48, 0},    {49, 0},    {50, 0},    {51, 0},    {52, 0},    {53, 0},
            {54, 0},    {55, 0},    {56, 0},    {57, 0},    {58, 0},    {59, 0},    {60, 0},    {61, 0},    {62, 0},
            {63, 0},    {64, 0},    {65, 0},    {66, 0},    {67, 0},    {68, 0},    {69, 0},    {70, 0},    {71, 0},
            {72, 0},    {73, 0},    {74, 0},    {75, 0},    {76, 0},    {77, 0},    {78, 0},    {79, 0},    {80, 0},
            {81, 0},    {82, 0},    {83, 0},    {84, 0},    {85, 0},    {86, 0},    {87, 0},    {88, 0},    {89, 0},
            {90, 0},    {91, 0},    {92, 0},    {93, 0},    {94, 0},    {95, 0},    {96, 0},    {97, 0},    {98, 0},
            {99, 0},    {100, 0},   {101, 0},   {102, 0},   {103, 0},   {104, 0},   {105, 0},   {106, 0},   {107, 0},
            {108, 0},   {109, 0},   {110, 0},   {111, 0},   {112, 0},   {113, 0},   {114, 0},   {115, 6},   {116, 0},
            {117, 0},   {118, 0},   {119, 0},   {120, 0},   {121, 0},   {122, 0},   {123, 0},   {124, 0},   {125, 0},
            {126, 0},   {127, 0},   {128, 0},   {129, 0},   {130, 0},   {131, 0},   {132, 0},   {133, 0},   {134, 0},
            {135, 0},   {136, 0},   {137, 0},   {138, 0},   {139, 0},   {140, 0},   {141, 0},   {142, 2},   {143, 0},
            {144, 0},   {145, 5},   {146, 0},   {147, 0},   {148, 0},   {149, 0},   {150, 0},   {151, 0},   {152, 0},
            {153, 0},   {154, 0},   {155, 0},   {156, 0},   {157, 0},   {158, 0},   {159, 0},   {160, 0},   {161, 0},
            {162, 0},   {163, 0},   {164, 0},   {165, 0},   {166, 0},   {167, 0},   {168, 0},   {169, 0},   {170, 0},
            {171, 0},   {172, 0},   {173, 0},   {174, 0},   {175, 0},   {176, 0},   {177, 0},   {178, 0},   {179, 0},
            {180, 0},   {181, 0},   {182, 4},   {183, 0},   {184, 0},   {185, 0},   {186, 0},   {187, 0},   {188, 0},
            {189, 0},   {190, 0},   {191, 0},   {192, 0},   {193, 0},   {194, 0},   {195, 0},   {196, 0},   {197, 0},
            {198, 0},   {199, 0},   {200, 0},   {201, 7},   {202, 0},   {203, 0},   {204, 0},   {205, 0},   {206, 0},
            {207, 0},   {208, 0},   {209, 0},   {210, 0},   {211, 0},   {212, 0},   {213, 0},   {214, 0},   {215, 0},
            {216, 0},   {217, 0},   {218, 0},   {219, 0},   {220, 0},   {221, 0},   {222, 0},   {223, 0},   {224, 0},
            {225, 0},   {226, 0},   {227, 0},   {228, 0},   {229, 0},   {230, 0},   {231, 0},   {232, 0},   {233, 0},
            {234, 0},   {235, 0},   {236, 0},   {237, 0},   {238, 0},   {239, 0},   {240, 0},   {241, 0},   {242, 0},
            {243, 0},   {244, 0},   {245, 0},   {246, 0},   {247, 0},   {248, 0},   {249, 0},   {250, 0},   {251, 0},
            {252, 0},   {253, 0},   {254, 0},   {255, 0},   {256, 0},   {257, 0},   {258, 0},   {259, 0},   {260, 0},
            {261, 0},   {262, 0},   {263, 0},   {264, 0},   {265, 0},   {266, 0},   {267, 0},   {268, 0},   {269, 0},
            {270, 0},   {271, 0},   {272, 0},   {273, 0},   {274, 0},   {275, 0},   {276, 0},   {277, 0},   {278, 0},
            {279, 0},   {280, 0},   {281, 0},   {282, 0},   {283, 0},   {284, 0},   {285, 0},   {286, 0},   {287, 0},
            {288, 0},   {289, 0},   {290, 0},   {291, 0},   {292, 0},   {293, 8},   {294, 0},   {295, 0},   {296, 0}};

        for (const auto& pool : dayPools1) {
                for (const auto& p : *pool) {
                        const auto dayId = p->GetPoolId(Id::Daycare);
                        if (AgeBrackets::Daycare::HasAge(p->GetAge())) {
                                EXPECT_NE(0, dayId);
                        } else {
                                EXPECT_EQ(0, dayId);
                        }
                }
        }

        for (const auto& pool : dayPools2) {
                for (const auto& p : *pool) {
                        const auto dayId = p->GetPoolId(Id::Daycare);
                        if (AgeBrackets::Daycare::HasAge(p->GetAge())) {
                                EXPECT_NE(0, dayId);
                        } else {
                                EXPECT_EQ(0, dayId);
                        }
                }
        }

        for (const auto& pool : dayPools3) {
                for (const auto& p : *pool) {
                        const auto dayId = p->GetPoolId(Id::Daycare);
                        if (AgeBrackets::Daycare::HasAge(p->GetAge())) {
                                EXPECT_NE(0, dayId);
                        } else {
                                EXPECT_EQ(0, dayId);
                        }
                }
        }
}

} // namespace
