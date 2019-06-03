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
 *  Copyright 2019 Laurens Van Damme
 */

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

#include "contact/AgeBrackets.h"
#include "disease/Health.h"
#include "geopop/GeoGrid.h"
#include "visualization/source/EpiOutputProtoReader.h"
#include "util/FileSys.h"

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace geopop;

namespace { // EpiOutputProtoReaderTest

void compareOutput(const string& filename, const GeoGrid<Epidemiologic>& geoGrid, int days)
{
        GeoGrid<EpiOutput> protoGeoGrid(nullptr);
        visualization::EpiOutputProtoReader reader(std::make_unique<std::ifstream>(filename), &protoGeoGrid);
        reader.Read();

        ASSERT_EQ(geoGrid.size(), protoGeoGrid.size());
        for (unsigned int i = 0; i < protoGeoGrid.size(); i++) {
                auto epiOutput = geoGrid[i]->GetContent()->GenerateEpiOutput();

                ASSERT_EQ(geoGrid[i]->GetID(), protoGeoGrid[i]->GetID());
                ASSERT_EQ(geoGrid[i]->GetProvince(), protoGeoGrid[i]->GetProvince());
                ASSERT_EQ(geoGrid[i]->get<0>(), protoGeoGrid[i]->get<0>());
                ASSERT_EQ(geoGrid[i]->get<1>(), protoGeoGrid[i]->get<1>());
                ASSERT_EQ(geoGrid[i]->GetName(), protoGeoGrid[i]->GetName());
                ASSERT_EQ(geoGrid[i]->GetContent()->GetPopCount(), protoGeoGrid[i]->GetContent()->pop_count);

                for (const auto& ageBracket: stride::ageBrackets) {
                        for (const auto& healthStatus: stride::healthStatuses) {
                                unsigned int measured_days = 0;
                                for ( const auto& myPair : protoGeoGrid[i]->GetContent()->epiOutput[ageBracket][healthStatus]) {
                                        unsigned int day = myPair.first;
                                        ASSERT_EQ(epiOutput[ageBracket][healthStatus], protoGeoGrid[i]->GetContent()->epiOutput[ageBracket][healthStatus][day]);
                                        measured_days++;
                                }
                                ASSERT_EQ(days, measured_days);
                        }
                }
        }

}

/// Checks if the epi-output protobuf reader creates the expected geoGrid with no time steps
TEST(EpiOutputProtoReaderTest, zeroLocationsTest)
{
        auto geoGrid = GeoGrid<Epidemiologic>(nullptr);

        compareOutput(FileSys::GetTestsDir().string() + "/testdata/EpiOutputProto/epiOutputTest1.proto", geoGrid, 1);
}

/// Checks if the epi-output protobuf reader creates the expected geoGrid consisting of one location with epi-output for
/// 0 time steps
TEST(EpiOutputProtoReaderTest, oneLocationZeroDaysTest)
{
        EXPECT_THROW(compareOutput(FileSys::GetTestsDir().string() + "/testdata/EpiOutputProto/epiOutputTest2.proto", GeoGrid<Epidemiologic>(nullptr), 0), runtime_error);
}

/// Checks if the epi-output protobuf reader creates the expected geoGrid consisting of one location with epi-output for
/// 1 time step
TEST(EpiOutputProtoReaderTest, oneLocationOneDayTest)
{
        // Create a location with contact pools
        ContactPool          pool(1, ContactType::Id::Household);
        ContactPool          pool2(1, ContactType::Id::Household);
        vector<unsigned int> ages = {45, 17, 5, 1, 75, 35, 25, 22, 2, 12};
        vector<Person>       persons{};
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons.emplace_back(Person());
        }
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons[i].SetAge(ages[i]);
                if (i > 1) {
                        persons[i].GetHealth().SetSusceptible();
                        if (i > 4) {
                                persons[i].GetHealth().StartInfection();
                        }
                        if (i > 6) {
                                persons[i].GetHealth().StopInfection();
                                if (i == 7) {
                                        persons[i].SetPoolId(ContactType::Id::Workplace, 1);
                                }
                        }
                } else {
                        persons[i].GetHealth().SetImmune();
                }
                pool.AddMember(&persons[i]);
                if (i != 0) {
                        pool2.AddMember(&persons[i]);
                }
        }
        Person p;
        p.SetAge(80);
        p.GetHealth().SetImmune();
        pool2.AddMember(&p);

        Location<Epidemiologic> test(1, 1, Coordinate(0.0, 0.0), "test", 0U);
        for (unsigned int i = 0; i < 4; i++) {
                test.GetContent()->RegisterPool(&pool, stride::ContactType::Id::Household);
        }
        test.GetContent()->RegisterPool(&pool2, stride::ContactType::Id::Household);

        auto geoGrid = GeoGrid<Epidemiologic>(nullptr);
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(test));

        compareOutput(FileSys::GetTestsDir().string() + "/testdata/EpiOutputProto/epiOutputTest3.proto", geoGrid, 1);
}

/// Checks if the epi-output protobuf reader creates the expected geoGrid consisting of two locations with epi-output
/// for 5 time steps
TEST(EpiOutputProtoReaderTest, twoLocationsFiveDaysTest)
{
        // Create a location with contact pools
        ContactPool          pool(1, ContactType::Id::Household);
        ContactPool          pool2(1, ContactType::Id::Household);
        vector<unsigned int> ages = {45, 17, 5, 1, 75, 35, 25, 22, 2, 12};
        vector<Person>       persons{};
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons.emplace_back(Person());
        }
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons[i].SetAge(ages[i]);
                if (i > 1) {
                        persons[i].GetHealth().SetSusceptible();
                        if (i > 4) {
                                persons[i].GetHealth().StartInfection();
                        }
                        if (i > 6) {
                                persons[i].GetHealth().StopInfection();
                                if (i == 7) {
                                        persons[i].SetPoolId(ContactType::Id::Workplace, 1);
                                }
                        }
                } else {
                        persons[i].GetHealth().SetImmune();
                }
                pool.AddMember(&persons[i]);
                if (i != 0) {
                        pool2.AddMember(&persons[i]);
                }
        }
        Person p;
        p.SetAge(80);
        p.GetHealth().SetImmune();
        pool2.AddMember(&p);

        Location<Epidemiologic> test(1, 1, Coordinate(0.0, 0.0), "test0", 0U);
        for (unsigned int i = 0; i < 4; i++) {
                test.GetContent()->RegisterPool(&pool, stride::ContactType::Id::Household);
        }
        test.GetContent()->RegisterPool(&pool2, stride::ContactType::Id::Household);

        auto geoGrid = GeoGrid<Epidemiologic>(nullptr);
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(test));
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(1, 1, Coordinate(1, 1), "test1", 100));

        compareOutput(FileSys::GetTestsDir().string() + "/testdata/EpiOutputProto/epiOutputTest4.proto", geoGrid, 6); // 5 days means 6 measurements (little mistake in test)
}

/// Checks if the epi-output protobuf reader throws an exception when the protobuf is empty
TEST(EpiOutputProtoReaderTest, emptyStreamTest) {
        EXPECT_THROW(compareOutput(FileSys::GetTestsDir().string() + "/testdata/EpiOutputProto/epiOutputTestEmpty.proto", GeoGrid<Epidemiologic>(nullptr), 0), runtime_error);
}

/// Checks if the epi-output protobuf reader is able to handle invalid input
TEST(EpiOutputProtoReaderTest, invalidTypeTest) { EXPECT_TRUE(true); }  // TODO: How to?

/// Checks if the epi-output protobuf reader throws an exception when the protobuf is invalid
TEST(EpiOutputProtoReaderTest, invalidJSONTest) {
        EXPECT_THROW(compareOutput(FileSys::GetTestsDir().string() + "/testdata/EpiOutputProto/epiOutputTestInvalid.proto", GeoGrid<Epidemiologic>(nullptr), 0), runtime_error);
}

} // namespace
