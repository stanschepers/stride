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

#include "Epidemiologic.h"
#include "contact/ContactPool.h"
#include "contact/ContactType.h"
#include "disease/Health.h"
#include "pop/Person.h"
#include "util/Exception.h"

#include <cmath>

namespace geopop {

    using namespace std;
    using namespace stride::ContactType;

    Epidemiologic::Epidemiologic(unsigned int popCount)
            : m_pop_count(popCount), m_pop_fraction(0.0), m_inCommutes(), m_outCommutes(), m_pool_index()
    {
    }

    bool Epidemiologic::operator==(const Epidemiologic& other) const
    {
        auto temp = true;
        for (Id typ : IdList) {
            temp = temp && (CRefPools(typ) == other.CRefPools(typ));
        }
        return temp && GetPopCount() == other.GetPopCount() &&
               CRefIncomingCommutes() == other.CRefIncomingCommutes() &&
               CRefOutgoingCommutes() == other.CRefOutgoingCommutes();
    }

    void Location::AddIncomingCommute(shared_ptr<Location> otherLocation, double fraction)
    {
        m_inCommutes.emplace_back(otherLocation.get(), fraction);
    }

    void Location::AddOutgoingCommute(shared_ptr<Location> otherLocation, double fraction)
    {
        m_outCommutes.emplace_back(otherLocation.get(), fraction);
    }

    int Location::GetIncomingCommuteCount(double fractionCommuters) const
    {
        double value = 0;
        for (const auto& locProportion : m_inCommutes) {
            // locProportion.second of the people in locProportion.first are commuting to this
            value += locProportion.second * (fractionCommuters * (double)locProportion.first->GetPopCount());
        }
        return static_cast<int>(floor(value));
    }

    unsigned int Location::GetInfectedCount() const
    {
        unsigned int total{0U};
        for (const auto& pool : CRefPools<Id::Household>()) {
            for (const auto& person : *pool) {
                const auto& h = person->GetHealth();
                total += h.IsInfected() || h.IsRecovered();
            }
        }
        return total;
    }

    unsigned int Location::GetOutgoingCommuteCount(double fractionCommuters) const
    {
        double totalProportion = 0;
        for (const auto& locProportion : m_outCommutes) {
            // locProportion.second of the people in this are commuting to locProportion.first
            totalProportion += locProportion.second;
        }
        return static_cast<unsigned int>(floor(totalProportion * (fractionCommuters * m_pop_count)));
    }

    unsigned int Location::GetMemberCount() const
    {
        unsigned int total_members = 0;
        for (stride::ContactPool* pool: this->CRefPools(stride::ContactType::Id::Household)){
            total_members += pool->size();
        }
        return total_members;
    }

    double Location::GetPopFraction() const { return m_pop_fraction; }

    std::map<std::string, std::map<std::string, double>> const Location::GenerateEpiOutput()
    {
        // Get the total amount of members in the location
        unsigned int total_members = this->GetMemberCount();

        // Initialize the map
        vector<string> ageBrackets = {"Daycare", "PreSchool", "K12School", "College", "Workplace", "Senior"};
        vector<string> healthCategories = {"Total", "Susceptible", "Infected", "Infectious", "Symptomatic", "Recovered",
                                           "Immune"};
        std::map<std::string, std::map<std::string, double>> epiOutput;
        for (const string &ageBracket: ageBrackets){
            for (const string &healthCategory: healthCategories){
                epiOutput[ageBracket][healthCategory] = 0;
            }
        }

        // Iterate over all Household contactpools in the location and calculate the epi-output
        for (stride::ContactPool* pool: this->CRefPools(stride::ContactType::Id::Household)){
            std::map<std::string, std::map<std::string, unsigned int>> epiOutput_pool = pool->GenerateEpiOutput();
            for (const string &ageBracket: ageBrackets){
                for (const string &healthCategory: healthCategories){
                    epiOutput[ageBracket][healthCategory] +=
                            double(epiOutput_pool[ageBracket][healthCategory]) / double(total_members);
                }
            }
        }

        return epiOutput;
    }

    void Location::SetPopCount(unsigned int totalPopCount)
    {
        m_pop_count = static_cast<unsigned int>(floor(m_pop_fraction * totalPopCount));
    }
    void Location::SetPopFraction(double relativePopulation) { m_pop_fraction = relativePopulation; }

} // namespace geopop
