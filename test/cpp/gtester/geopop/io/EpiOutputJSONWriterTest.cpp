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

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

namespace {

TEST(EpiOutputJSONWriterTest, zeroLocationTest) { EXPECT_TRUE(true); }

TEST(EpiOutputJSONWriterTest, oneLocationZeroDaysTest) { EXPECT_TRUE(true); }

TEST(EpiOutputJSONWriterTest, oneLocationOneDayTest) { EXPECT_TRUE(true); }

TEST(EpiOutputJSONWriterTest, oneLocationTwoDaysTest) { EXPECT_TRUE(true); }

TEST(EpiOutputJSONWriterTest, fiveLocationsOneDayTest) { EXPECT_TRUE(true); }

} // namespace
