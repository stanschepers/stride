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

//#include "geopop/io/WorkplaceFormatReader.h"
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
            R"(ratio, size_min, size_max
               0.77,  1,  9
               0.17, 10, 49
               0.04, 50,199
               0.02,200,400
            )";

        GeoGridConfig      geoConfig{};
        auto               instream = make_unique<istringstream>(csvString);
//        WorkplaceFormatReader reader(move(instream));


        EXPECT_EQ(true, false);
}

} // namespace
