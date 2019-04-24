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

//#include "geopop/populators/WorkInstancePopulator.h"

#include "contact/AgeBrackets.h"
//#include "createGeogrid.h"
#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
//#include "geopop/HouseholdCenter.h"
//#include "geopop/WorkplaceCenter.h"
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

TEST(WorkInstancePopulatorTest, NoPopulation)
{
        EXPECT_EQ(true, false);
}

TEST(WorkInstancePopulatorTest, OneLocationTest)
{
        EXPECT_EQ(true, false);
}

TEST(WorkInstancePopulatorTest, TwoLocationTest)
{
        EXPECT_EQ(true, false);
}

} // namespace
