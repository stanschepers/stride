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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J, Van Damme L
 */

/**
 * @file
 * Implementation for the core ContcatPool class.
 */

#include "ContactPool.h"

#include "disease/Health.h"
#include "AgeBrackets.h"
#include "pop/Age.h"
#include "pop/Person.h"

#include <algorithm>

namespace stride {

using namespace std;

ContactPool::ContactPool(unsigned int poolId, ContactType::Id type)
    : m_index_immune(0), m_pool_id(poolId), m_pool_type(type), m_members()
{
}

void ContactPool::AddMember(Person* p)
{
        m_members.emplace_back(p);
        m_index_immune++;
}

unsigned int ContactPool::GetInfectedCount() const
{
        unsigned int infected = 0;

        for (stride::Person* person : m_members) {
                if (person->GetHealth().IsInfected()) {
                        infected++;
                }
        }
        return infected;
}

const std::map<std::string, std::map<std::string, unsigned int>> ContactPool::GenerateEpiOutput()
{
        // Prepare a map
        map<string, map<string, unsigned int>> epiOutput;
        for (const string& ageBracket : stride::ageBrackets) {
                for (const string& healthStatus : stride::healthStatuses) {
                        epiOutput[ageBracket][healthStatus] = 0U;
                }
        }

        // Iterate over all members and add 1 to the right health categories
        for (Person* member : m_members) {
                string ageBracket;
                double age = member->GetAge();
                if (AgeBrackets::Daycare::HasAge(age)) {
                        ageBracket = "Daycare";
                } else if (AgeBrackets::PreSchool::HasAge(age)) {
                        ageBracket = "PreSchool";
                } else if (AgeBrackets::K12School::HasAge(age)) {
                        ageBracket = "K12School";
                } else if (AgeBrackets::College::HasAge(age) && member->GetPoolId(ContactType::Id::Workplace) == 0) {
                        ageBracket = "College";
                } else if (AgeBrackets::Workplace::HasAge(age)) {
                        ageBracket = "Workplace";
                } else {
                        ageBracket = "Senior";
                }
                epiOutput[ageBracket]["Total"] += 1;

                if (member->GetHealth().IsSusceptible()) {
                        epiOutput[ageBracket]["Susceptible"] += 1;
                }
                if (member->GetHealth().IsInfected()) {
                        epiOutput[ageBracket]["Infected"] += 1;
                }
                if (member->GetHealth().IsInfectious()) {
                        epiOutput[ageBracket]["Infectious"] += 1;
                }
                if (member->GetHealth().IsSymptomatic()) {
                        epiOutput[ageBracket]["Symptomatic"] += 1;
                }
                if (member->GetHealth().IsRecovered()) {
                        epiOutput[ageBracket]["Recovered"] += 1;
                }
                if (member->GetHealth().IsImmune()) {
                        epiOutput[ageBracket]["Immune"] += 1;
                }
        }

        return epiOutput;
}

std::tuple<bool, unsigned int> ContactPool::SortMembers()
{
        bool         infectious_cases = false;
        unsigned int num_cases        = 0;

        for (size_t i_member = 0; i_member < m_index_immune; i_member++) {
                // if immune, move to back
                if (m_members[i_member]->GetHealth().IsImmune()) {
                        bool         swapped   = false;
                        unsigned int new_place = m_index_immune - 1;
                        m_index_immune--;
                        while (!swapped && new_place > i_member) {
                                if (m_members[new_place]->GetHealth().IsImmune()) {
                                        m_index_immune--;
                                        new_place--;
                                } else {
                                        swap(m_members[i_member], m_members[new_place]);
                                        swapped = true;
                                }
                        }
                }
                // else, if not susceptible, move to front
                else if (!m_members[i_member]->GetHealth().IsSusceptible()) {
                        if (!infectious_cases && m_members[i_member]->GetHealth().IsInfectious()) {
                                infectious_cases = true;
                        }
                        if (i_member > num_cases) {
                                swap(m_members[i_member], m_members[num_cases]);
                        }
                        num_cases++;
                }
        }
        return std::make_tuple(infectious_cases, num_cases);
}

} // namespace stride
