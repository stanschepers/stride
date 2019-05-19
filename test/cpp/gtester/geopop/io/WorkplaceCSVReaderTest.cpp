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

#include "geopop/io/WorkplaceCSVReader.h"
#include "geopop/GeoGridConfig.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace std;
using namespace geopop;
using namespace stride;

namespace {

TEST(WorkplaceFormatReader, test1)
{
        string csvString =
            "ratio; size_min; size_max\r"
            "0.77;1;9\r"
            "0.17;10;49\r"
            "0.04;50;199\r"
            "0.02;200;400\r";

        GeoGridConfig      geoConfig{};
        auto               instream = make_unique<istringstream>(csvString);
        WorkplaceCSVReader reader(move(instream));

        reader.SetWorkDistribution(geoConfig.param.work_distribution);

        auto distribution = geoConfig.param.work_distribution;

        EXPECT_EQ(distribution.size(), 4U);

        EXPECT_DOUBLE_EQ(std::get<0>(distribution[0]), 0.77L);
        EXPECT_EQ(std::get<1>(distribution[0]), 1U);
        EXPECT_EQ(std::get<2>(distribution[0]), 9U);

        EXPECT_DOUBLE_EQ(std::get<0>(distribution[1]), 0.17L);
        EXPECT_EQ(std::get<1>(distribution[1]), 10U);
        EXPECT_EQ(std::get<2>(distribution[1]), 49U);

        EXPECT_DOUBLE_EQ(std::get<0>(distribution[2]), 0.04L);
        EXPECT_EQ(std::get<1>(distribution[2]), 50U);
        EXPECT_EQ(std::get<2>(distribution[2]), 199U);

        EXPECT_DOUBLE_EQ(std::get<0>(distribution[3]), 0.02L);
        EXPECT_EQ(std::get<1>(distribution[3]), 200U);
        EXPECT_EQ(std::get<2>(distribution[3]), 400U);
}

} // namespace
