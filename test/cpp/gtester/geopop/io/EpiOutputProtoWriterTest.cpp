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

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "geopop/GeoGrid.h"
#include "geopop/io/EpiOutputProtoWriter.h"
#include "contact/AgeBrackets.h"
#include "disease/Health.h"

using namespace std;
using namespace stride;
using namespace geopop;

namespace { // EpiOutputProtoWriterTest

void compareOutput(stringstream& ss, const GeoGrid<Epidemiologic>& geoGrid, int days)
{
    proto::EpiOutput protoEpiOutput;
    protoEpiOutput.ParseFromIstream(&ss);

    ASSERT_EQ(days, protoEpiOutput.days_size());
    for (int l = 0; l < protoEpiOutput.days_size(); l++) {
        const auto& protoDay = protoEpiOutput.days(l);
        ASSERT_EQ(protoDay.locations_size(), geoGrid.size());
        for (int i = 0; i < protoDay.locations_size(); i++) {
            const auto& protoLoc  = protoDay.locations(i);
            const auto& protoCoor = protoLoc.coordinate();

            auto epiOutput = geoGrid[i]->GetContent()->GenerateEpiOutput();
            
            ASSERT_EQ(geoGrid[i]->GetID(), protoLoc.id());
            ASSERT_EQ(geoGrid[i]->GetProvince(), protoLoc.province());
            ASSERT_EQ(geoGrid[i]->get<0>(), protoCoor.latitude());
            ASSERT_EQ(geoGrid[i]->get<1>(), protoCoor.longitude());
            ASSERT_EQ(geoGrid[i]->GetName(), protoLoc.name());
            ASSERT_EQ(geoGrid[i]->GetContent()->GetPopCount(), protoLoc.population());
            
            ASSERT_EQ(stride::ageBrackets.size(), protoLoc.agebrackets_size());
            for (int j = 0; j < protoLoc.agebrackets_size(); j++) {
                const auto& protoAgeBracket = protoLoc.agebrackets(j);
                
                ASSERT_EQ(stride::healthStatuses.size(), protoAgeBracket.healthstatuses_size());
                for (int k = 0; k < protoAgeBracket.healthstatuses_size(); k++) {
                    const auto& protoHealthStatus = protoAgeBracket.healthstatuses(k);
                    
                    ASSERT_EQ(epiOutput[protoAgeBracket.type()][protoHealthStatus.type()], protoHealthStatus.percentage());
                }
            }
        }
    }
}

/// Checks if the epi-output protobuf writer creates the expected result with no locations in geogrid
TEST(EpiOutputProtoWriterTest, zeroLocationsTest)
{
        auto geoGrid = GeoGrid<Epidemiologic>(nullptr);

        auto writer = EpiOutputProtoWriter();

        writer.Update(geoGrid, 0);

        stringstream ss;
        writer.Write(ss);

        compareOutput(ss, geoGrid, 1);
}

/// Checks if the epi-output protobuf writer creates the expected result with no time steps
TEST(EpiOutputProtoWriterTest, oneLocationZeroDaysTest)
{
        auto writer = EpiOutputProtoWriter();

        stringstream ss;
        writer.Write(ss);

        compareOutput(ss, GeoGrid<Epidemiologic>(nullptr), 0);
}

/// Checks if the epi-output protobuf writer creates the expected result with a geogrid consisting of one location and over 1 time step
TEST(EpiOutputProtoWriterTest, oneLocationOneDayTest)
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

        auto writer = EpiOutputProtoWriter();

        writer.Update(geoGrid, 0);

        stringstream ss;
        writer.Write(ss);

        compareOutput(ss, geoGrid, 1);
}

/// Checks if the epi-output protobuf writer creates the expected result with a geogrid consisting of two location and over 5 time steps
TEST(EpiOutputProtoWriterTest, twoLocationsFiveDaysTest)
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

        auto writer = EpiOutputProtoWriter();

        for (unsigned int i = 0; i < 51; i += 10)
                writer.Update(geoGrid, i);

        stringstream ss;
        writer.Write(ss);

        compareOutput(ss, geoGrid, 6); // 5 days means 6 measurements (little mistake in test)
}

} // namespace
