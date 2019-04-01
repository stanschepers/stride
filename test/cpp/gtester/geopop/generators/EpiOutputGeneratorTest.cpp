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
#include "pop/Person.h"

#include <gtest/gtest.h>
#include <map>
#include <vector>

using namespace std;
using namespace stride;

namespace {
    vector<string> ageBrackets = {"Daycare", "PreSchool", "K12School", "College", "Workplace", "Senior"};
    vector<string> healthCategories = {"Total", "Susceptible", "Infected", "Infectious", "Symptomatic", "Recovered", "Immune"};

    // Checks if the function generateEpiOutput of the class Contactpool returns the expected result with no members in the contactpool.
    TEST(EpiOutputGeneratorTest, contactpoolZeroMembersTest)
    {
        map<string, map<string, unsigned int>> correct_result;
        for (string s1: ageBrackets){
            for (string s2: healthCategories){
                correct_result[s1][s2] = 0;
            }
        }

        ContactPool test(1, ContactType::Id::Household);

        map<string, map<string, unsigned int>> result = test.generateEpiOutput();

        EXPECT_TRUE(correct_result.size() == result.size()
                    && std::equal(correct_result.begin(), correct_result.end(),
                                  result.begin()));
    }

    // Checks if the function generateEpiOutput of the class Contactpool returns the expected result with one member in the contactpool.
    TEST(EpiOutputGeneratorTest, contactpoolOneMemberTest)
    {
        map<string, map<string, unsigned int>> correct_result;
        for (string s1: ageBrackets){
            for (string s2: healthCategories){
                correct_result[s1][s2] = 0;
            }
        }

        correct_result["College"]["Total"] = 1;
        correct_result["College"]["Infected"] = 1;

        Person p;
        p.SetAge(20);
        p.GetHealth().SetSusceptible();
        p.GetHealth().StartInfection();
        ContactPool test(1, ContactType::Id::Household);
        test.AddMember(&p);

        map<string, map<string, unsigned int>> result = test.generateEpiOutput();

        EXPECT_TRUE(correct_result.size() == result.size()
                    && std::equal(correct_result.begin(), correct_result.end(),
                                  result.begin()));
    }

    // Checks if the function generateEpiOutput of the class Contactpool returns the expected result with five members in the contactpool.
    TEST(EpiOutputGeneratorTest, contactpoolFiveMembersTest)
    {
        map<string, map<string, unsigned int>> correct_result;
        for (string s1: ageBrackets){
            for (string s2: healthCategories){
                if (s2 != "Total" || s1 == "College"){
                    correct_result[s1][s2] = 0;
                } else {
                    correct_result[s1][s2] = 1;
                }
            }
        }

        correct_result["Daycare"]["Recovered"] = 1;
        correct_result["PreSchool"]["Infected"] = 1;
        correct_result["K12School"]["Susceptible"] = 1;
        correct_result["Workplace"]["Immune"] = 1;
        correct_result["Senior"]["Recovered"] = 1;

        ContactPool test(1, ContactType::Id::Household);
        vector<unsigned int> ages = {45, 17, 5, 1, 75};
        vector<Person> persons = {Person(), Person(), Person(), Person(), Person()};
        for (int i = 0; i < persons.size(); i++) {
            persons[i].SetAge(ages[i]);
            if (i != 0){
                persons[i].GetHealth().SetSusceptible();
                if (i > 1) {
                    persons[i].GetHealth().StartInfection();
                }
                if (i > 2) {
                    persons[i].GetHealth().StopInfection();
                }
            } else {
                persons[i].GetHealth().SetImmune();
            }
            test.AddMember(&persons[i]);
        }

        map<string, map<string, unsigned int>> result = test.generateEpiOutput();

        EXPECT_TRUE(correct_result.size() == result.size()
                    && std::equal(correct_result.begin(), correct_result.end(),
                                  result.begin()));
    }

    TEST(EpiOutputGeneratorTest, locationZeroContactpoolsTest)
    {
        EXPECT_TRUE(false);
    }

    TEST(EpiOutputGeneratorTest, locationOneContactpoolTest)
    {
        EXPECT_TRUE(false);
    }

    TEST(EpiOutputGeneratorTest, locationFiveContactpoolsTest)
    {
        EXPECT_TRUE(false);
    }

} // namespace

//correct_result["Daycare"]["Total"] = 0;
//correct_result["Daycare"]["Susceptible"] = 0;
//correct_result["Daycare"]["Infected"] = 0;
//correct_result["Daycare"]["Infectious"] = 0;
//correct_result["Daycare"]["Symptomatic"] = 0;
//correct_result["Daycare"]["Recovered"] = 0;
//correct_result["Daycare"]["Immune"] = 0;
//
//correct_result["PreSchool"]["Total"] = 0;
//correct_result["PreSchool"]["Susceptible"] = 0;
//correct_result["PreSchool"]["Infected"] = 0;
//correct_result["PreSchool"]["Infectious"] = 0;
//correct_result["PreSchool"]["Symptomatic"] = 0;
//correct_result["PreSchool"]["Recovered"] = 0;
//correct_result["PreSchool"]["Immune"] = 0;
//
//correct_result["K12School"]["Total"] = 0;
//correct_result["K12School"]["Susceptible"] = 0;
//correct_result["K12School"]["Infected"] = 0;
//correct_result["K12School"]["Infectious"] = 0;
//correct_result["K12School"]["Symptomatic"] = 0;
//correct_result["K12School"]["Recovered"] = 0;
//correct_result["K12School"]["Immune"] = 0;
//
//correct_result["College"]["Total"] = 0;
//correct_result["College"]["Susceptible"] = 0;
//correct_result["College"]["Infected"] = 0;
//correct_result["College"]["Infectious"] = 0;
//correct_result["College"]["Symptomatic"] = 0;
//correct_result["College"]["Recovered"] = 0;
//correct_result["College"]["Immune"] = 0;
//
//correct_result["Workplace"]["Total"] = 0;
//correct_result["Workplace"]["Susceptible"] = 0;
//correct_result["Workplace"]["Infected"] = 0;
//correct_result["Workplace"]["Infectious"] = 0;
//correct_result["Workplace"]["Symptomatic"] = 0;
//correct_result["Workplace"]["Recovered"] = 0;
//correct_result["Workplace"]["Immune"] = 0;
//
//correct_result["Senior"]["Total"] = 0;
//correct_result["Senior"]["Susceptible"] = 0;
//correct_result["Senior"]["Infected"] = 0;
//correct_result["Senior"]["Infectious"] = 0;
//correct_result["Senior"]["Symptomatic"] = 0;
//correct_result["Senior"]["Recovered"] = 0;
//correct_result["Senior"]["Immune"] = 0;
