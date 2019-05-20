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
 *  Copyright 2019, Jan Broeckhove and Bistromatics group.
 */

#include "contact/ContactPool.h"
#include "contact/AgeBrackets.h"
#include "geopop/Coordinate.h"
#include "geopop/Location.h"
#include "geopop/Epidemiologic.h"
#include "pop/Person.h"
#include "disease/Health.h"

#include <gtest/gtest.h>
#include <map>
#include <vector>

using namespace std;
using namespace stride;
using namespace geopop;

bool compareMapWithDoubles(map<string, map<string, double>> map1, map<string, map<string, double>> map2)
{
        for (const string& ageBracket : stride::ageBrackets) {
                for (const string& healtCategory : stride::healthStatuses) {
                        if (map1[ageBracket][healtCategory] - map2[ageBracket][healtCategory] > 0.0000000000001)
                                return false;
                }
        }
        return true;
}

namespace {

/// Checks if the function generateEpiOutput of the class Contactpool returns the expected result with no members in the
/// contactpool.
TEST(EpiOutputGeneratorTest, contactpoolZeroMembersTest)
{
        map<string, map<string, unsigned int>> correct_result;
        for (const string& ageBracket : stride::ageBrackets) {
                for (const string& healthStatus : stride::healthStatuses) {
                        correct_result[ageBracket][healthStatus] = 0;
                }
        }

        ContactPool test(1, ContactType::Id::Household);

        map<string, map<string, unsigned int>> result = test.GenerateEpiOutput();

        EXPECT_TRUE(correct_result.size() == result.size() &&
                    std::equal(correct_result.begin(), correct_result.end(), result.begin()));
}

/// Checks if the function generateEpiOutput of the class Contactpool returns the expected result with one member in the
/// contactpool.
TEST(EpiOutputGeneratorTest, contactpoolOneMemberTest)
{
        map<string, map<string, unsigned int>> correct_result;
        for (const string& ageBracket : stride::ageBrackets) {
                for (const string& healthStatus : stride::healthStatuses) {
                        correct_result[ageBracket][healthStatus] = 0;
                }
        }

        correct_result["College"]["Total"]    = 1;
        correct_result["College"]["Infected"] = 1;

        Person p;
        p.SetAge(20);
        p.GetHealth().SetSusceptible();
        p.GetHealth().StartInfection();
        ContactPool test(1, ContactType::Id::Household);
        test.AddMember(&p);

        map<string, map<string, unsigned int>> result = test.GenerateEpiOutput();

        EXPECT_TRUE(correct_result.size() == result.size() &&
                    std::equal(correct_result.begin(), correct_result.end(), result.begin()));
}

/// Checks if the function generateEpiOutput of the class Contactpool returns the expected result with five members in
/// the contactpool.
TEST(EpiOutputGeneratorTest, contactpoolFiveMembersTest)
{
        map<string, map<string, unsigned int>> correct_result;
        for (const string& ageBracket : stride::ageBrackets) {
                for (const string& healthStatus : stride::healthStatuses) {
                        if (healthStatus != "Total" || ageBracket == "College") {
                                correct_result[ageBracket][healthStatus] = 0;
                        } else {
                                correct_result[ageBracket][healthStatus] = 1;
                        }
                }
        }

        correct_result["Daycare"]["Recovered"]     = 1;
        correct_result["PreSchool"]["Infected"]    = 1;
        correct_result["K12School"]["Susceptible"] = 1;
        correct_result["Workplace"]["Immune"]      = 1;
        correct_result["Senior"]["Recovered"]      = 1;

        ContactPool          test(1, ContactType::Id::Household);
        vector<unsigned int> ages = {22, 17, 5, 1, 75};
        vector<Person>       persons{};
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons.emplace_back(Person());
        }
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons[i].SetAge(ages[i]);
                if (i != 0) {
                        persons[i].GetHealth().SetSusceptible();
                        if (i > 1) {
                                persons[i].GetHealth().StartInfection();
                                if (i > 2) {
                                        persons[i].GetHealth().StopInfection();
                                }
                        }
                } else {
                        persons[i].GetHealth().SetImmune();
                        persons[i].SetPoolId(ContactType::Id::Workplace, 1);
                }
                test.AddMember(&persons[i]);
        }

        map<string, map<string, unsigned int>> result = test.GenerateEpiOutput();

        EXPECT_TRUE(correct_result.size() == result.size() &&
                    std::equal(correct_result.begin(), correct_result.end(), result.begin()));
}

/// Checks if the function generateEpiOutput of the class Location returns the expected result with no contactpools in
/// the location.
TEST(EpiOutputGeneratorTest, locationZeroContactpoolsTest)
{
        map<string, map<string, double>> correct_result;
        for (const string& ageBracket : stride::ageBrackets) {
                for (const string& healthStatus : stride::healthStatuses) {
                        correct_result[ageBracket][healthStatus] = 0;
                }
        }

        Location<Epidemiologic> test(1, 1, Coordinate(0.0, 0.0), "test", 0U);

        map<string, map<string, double>> result = test.GetContent()->GenerateEpiOutput();

        EXPECT_TRUE(correct_result.size() == result.size() && compareMapWithDoubles(result, correct_result));
}

/// Checks if the function generateEpiOutput of the class Location returns the expected result with one contactpool in
/// the location.
TEST(EpiOutputGeneratorTest, locationOneContactpoolTest)
{
        map<string, map<string, double>> correct_result;
        for (const string& ageBracket : stride::ageBrackets) {
                for (const string& healthStatus : stride::healthStatuses) {
                        if (healthStatus != "Total" || ageBracket == "College") {
                                correct_result[ageBracket][healthStatus] = 0;
                        } else {
                                correct_result[ageBracket][healthStatus] = 0.2;
                        }
                }
        }

        correct_result["Daycare"]["Recovered"]     = 0.2;
        correct_result["PreSchool"]["Infected"]    = 0.2;
        correct_result["K12School"]["Susceptible"] = 0.2;
        correct_result["Workplace"]["Immune"]      = 0.2;
        correct_result["Senior"]["Recovered"]      = 0.2;

        ContactPool          pool(1, ContactType::Id::Household);
        vector<unsigned int> ages = {22, 17, 5, 1, 75};
        vector<Person>       persons{};
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons.emplace_back(Person());
        }
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons[i].SetAge(ages[i]);
                if (i != 0) {
                        persons[i].GetHealth().SetSusceptible();
                        if (i > 1) {
                                persons[i].GetHealth().StartInfection();
                                if (i > 2) {
                                        persons[i].GetHealth().StopInfection();
                                }
                        }
                } else {
                        persons[i].GetHealth().SetImmune();
                        persons[i].SetPoolId(ContactType::Id::Workplace, 1);
                }
                pool.AddMember(&persons[i]);
        }

        Location<Epidemiologic> test(1, 1, Coordinate(0.0, 0.0), "test", 0U);
        test.GetContent()->RegisterPool(&pool, stride::ContactType::Id::Household);

        map<string, map<string, double>> result = test.GetContent()->GenerateEpiOutput();

        EXPECT_TRUE(correct_result.size() == result.size() && compareMapWithDoubles(result, correct_result));
}

/// Checks if the function generateEpiOutput of the class Location returns the expected result with five contactpools in
/// the location.
TEST(EpiOutputGeneratorTest, locationFiveContactpoolsTest)
{
        map<string, map<string, double>> correct_result;
        for (const string& ageBracket : stride::ageBrackets) {
                for (const string& healthStatus : stride::healthStatuses) {
                        correct_result[ageBracket][healthStatus] = 0;
                }
        }

        correct_result["Daycare"]["Total"]       = 0.2;
        correct_result["Daycare"]["Susceptible"] = 0.1;
        correct_result["Daycare"]["Recovered"]   = 0.1;

        correct_result["PreSchool"]["Total"]       = 0.1;
        correct_result["PreSchool"]["Susceptible"] = 0.1;

        correct_result["K12School"]["Total"]     = 0.2;
        correct_result["K12School"]["Recovered"] = 0.1;
        correct_result["K12School"]["Immune"]    = 0.1;

        correct_result["College"]["Total"]    = 0.1;
        correct_result["College"]["Infected"] = 0.1;

        correct_result["Workplace"]["Total"]     = 0.28;
        correct_result["Workplace"]["Infected"]  = 0.1;
        correct_result["Workplace"]["Recovered"] = 0.1;
        correct_result["Workplace"]["Immune"]    = 0.8;

        correct_result["Senior"]["Total"]       = 0.12;
        correct_result["Senior"]["Susceptible"] = 0.1;
        correct_result["Senior"]["Immune"]      = 0.02;

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

        map<string, map<string, double>> result = test.GetContent()->GenerateEpiOutput();

        EXPECT_TRUE(correct_result.size() == result.size() && compareMapWithDoubles(result, correct_result));
}

} // namespace

// correct_result["Daycare"]["Total"] = 0;
// correct_result["Daycare"]["Susceptible"] = 0;
// correct_result["Daycare"]["Infected"] = 0;
// correct_result["Daycare"]["Infectious"] = 0;
// correct_result["Daycare"]["Symptomatic"] = 0;
// correct_result["Daycare"]["Recovered"] = 0;
// correct_result["Daycare"]["Immune"] = 0;
//
// correct_result["PreSchool"]["Total"] = 0;
// correct_result["PreSchool"]["Susceptible"] = 0;
// correct_result["PreSchool"]["Infected"] = 0;
// correct_result["PreSchool"]["Infectious"] = 0;
// correct_result["PreSchool"]["Symptomatic"] = 0;
// correct_result["PreSchool"]["Recovered"] = 0;
// correct_result["PreSchool"]["Immune"] = 0;
//
// correct_result["K12School"]["Total"] = 0;
// correct_result["K12School"]["Susceptible"] = 0;
// correct_result["K12School"]["Infected"] = 0;
// correct_result["K12School"]["Infectious"] = 0;
// correct_result["K12School"]["Symptomatic"] = 0;
// correct_result["K12School"]["Recovered"] = 0;
// correct_result["K12School"]["Immune"] = 0;
//
// correct_result["College"]["Total"] = 0;
// correct_result["College"]["Susceptible"] = 0;
// correct_result["College"]["Infected"] = 0;
// correct_result["College"]["Infectious"] = 0;
// correct_result["College"]["Symptomatic"] = 0;
// correct_result["College"]["Recovered"] = 0;
// correct_result["College"]["Immune"] = 0;
//
// correct_result["Workplace"]["Total"] = 0;
// correct_result["Workplace"]["Susceptible"] = 0;
// correct_result["Workplace"]["Infected"] = 0;
// correct_result["Workplace"]["Infectious"] = 0;
// correct_result["Workplace"]["Symptomatic"] = 0;
// correct_result["Workplace"]["Recovered"] = 0;
// correct_result["Workplace"]["Immune"] = 0;
//
// correct_result["Senior"]["Total"] = 0;
// correct_result["Senior"]["Susceptible"] = 0;
// correct_result["Senior"]["Infected"] = 0;
// correct_result["Senior"]["Infectious"] = 0;
// correct_result["Senior"]["Symptomatic"] = 0;
// correct_result["Senior"]["Recovered"] = 0;
// correct_result["Senior"]["Immune"] = 0;
