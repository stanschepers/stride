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

#include "Populator.h"

#include "contact/AgeBrackets.h"
#include "contact/ContactPool.h"
#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "util/Assert.h"

#include <utility>
#include <map>
#include <vector>

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::AgeBrackets;
using namespace util;

template<>
void Populator<stride::ContactType::Id::Workplace>::Apply(GeoGrid& geoGrid, const GeoGridConfig& geoGridConfig)
{
        m_logger->trace("Starting to populate Workplaces");

        auto genCommute{function<int()>()};
        auto genNonCommute{function<int()>()};
        vector<ContactPool*> nearbyWp{};
        vector<Location*> commuteLocations{};

        const auto participCollege      = geoGridConfig.param.participation_college;
        const auto participWorkplace    = geoGridConfig.param.particpation_workplace;
        const auto popCollege           = geoGridConfig.info.popcount_college;
        const auto popWorkplace         = geoGridConfig.info.popcount_workplace;
        const auto fracCollegeCommute   = geoGridConfig.param.fraction_college_commuters;
        const auto fracWorkplaceCommute = geoGridConfig.param.fraction_workplace_commuters;
        const auto distribution         = geoGridConfig.param.work_distribution;

        int total = 0;
        int com = 0;
        int noncom = 0;

        double fracCommuteStudents = 0.0;
        if (static_cast<bool>(fracWorkplaceCommute) && popWorkplace) {
                fracCommuteStudents = (popCollege * fracCollegeCommute) /(popWorkplace * fracWorkplaceCommute);
        }

        // --------------------------------------------------------------------------------
        // For every location, if populated ...
        // --------------------------------------------------------------------------------
        for (const auto& loc : geoGrid) {
                if (loc->GetPopCount() == 0) {
                        continue;
                }

                // --------------------------------------------------------------------------------
                // Find all Workplaces were employees from this location commute to
                // --------------------------------------------------------------------------------
                commuteLocations.clear();
                genCommute = function<int()>();

                vector<double> commutingWeights;
                for (const pair<Location*, double>& commute : loc->CRefOutgoingCommutes()) {
                        const auto& workplaces = commute.first->RefPools(Id::Workplace);
                        if (!workplaces.empty()) {
                                commuteLocations.push_back(commute.first);
                                const auto weight = commute.second - (commute.second * fracCommuteStudents);
                                commutingWeights.push_back(weight);
                                AssertThrow(weight >= 0.0 && weight <= 1.0 && !isnan(weight),
                                            "Invalid weight: " + to_string(weight), m_logger);
                        }
                }
                auto refCommutingWeights = commutingWeights;
                auto refCommutingLocations = commuteLocations;

                if (!commutingWeights.empty()) {
                        genCommute = m_rn_man.GetDiscreteGenerator(commutingWeights, 0U);
                }

                // --------------------------------------------------------------------------------
                // Set NearbyWorkspacePools and associated generator
                // --------------------------------------------------------------------------------
                nearbyWp      = geoGrid.GetNearbyPools(Id::Workplace, *loc);


                // --------------------------------------------------------------------------------
                // For everyone of working age: decide between work or college (iff of College age)
                // --------------------------------------------------------------------------------
                for (auto& hhPool : loc->RefPools(Id::Household)) {
                        for (unsigned int i=0; i<unsigned(int((*hhPool).size())); i++) {
                                auto person = (*hhPool)[i];
                                if (!Workplace::HasAge(person->GetAge())) {
                                        continue;
                                }

                                bool isStudent      = m_rn_man.MakeWeightedCoinFlip(participCollege);
                                bool isActiveWorker = m_rn_man.MakeWeightedCoinFlip(participWorkplace);

                                if ((College::HasAge(person->GetAge()) && !isStudent) || isActiveWorker) {
                                        // ---------------------------------------------
                                        // this person is employed
                                        // ---------------------------------------------

                                        total++;

                                        const auto isCommuter = m_rn_man.MakeWeightedCoinFlip(fracWorkplaceCommute);
                                        if (!commuteLocations.empty() && isCommuter) {
                                                // --------------------------------------------------------------
                                                // this person commutes to the Location and in particular to Pool
                                                // --------------------------------------------------------------
                                                auto comDraw = genCommute();
                                                auto& pools  = commuteLocations[comDraw]->RefPools(Id::Workplace);
                                                auto  s      = static_cast<unsigned int>(pools.size());
                                                auto  gen    = m_rn_man.GetUniformIntGenerator(0, s);

                                                auto firstGenDraw = gen();
                                                auto genDraw = firstGenDraw;
                                                while (pools[genDraw]->CheckLimit()) {
                                                        genDraw++;
                                                        genDraw = genDraw % s;

                                                        if(genDraw == firstGenDraw){
                                                                // ----------------------------
                                                                // This commuteLocation is FULL
                                                                // ----------------------------
//                                                                commutingWeights[comDraw] = double(0);
                                                                commutingWeights.erase(commutingWeights.begin() + comDraw);
                                                                commuteLocations.erase(commuteLocations.begin() + comDraw);
                                                                if (commutingWeights.empty()){
                                                                        com++;
                                                                        genDraw = gen();
                                                                        break;
                                                                }
                                                                genCommute = m_rn_man.GetDiscreteGenerator(commutingWeights, 0U);

                                                                vector<Location*> vecLocs;
                                                                for (auto g : commuteLocations){
                                                                        vecLocs.push_back(g);
//                                                                        std::cout << g << std::endl;
                                                                }
                                                                comDraw = genCommute();

                                                                pools  = commuteLocations[comDraw]->RefPools(Id::Workplace);
                                                                s      = static_cast<unsigned int>(pools.size());
                                                                gen    = m_rn_man.GetUniformIntGenerator(0, s);
                                                                firstGenDraw = gen();
                                                                genDraw = firstGenDraw;
                                                        }
                                                }
                                                auto pool    = pools[genDraw];

                                                // so that's it
                                                pool->AddMember(person);
                                                person->SetPoolId(Id::Workplace, pool->GetId());
                                        } else {
                                                // ----------------------------
                                                // this person does not commute
                                                // ----------------------------
                                                genNonCommute = m_rn_man.GetUniformIntGenerator(0, static_cast<int>(nearbyWp.size()), 0U);

//                                                unsigned int attempts = 0;
//                                                static int count = 0;
//                                                while(pool->CheckLimit() and attempts < static_cast<unsigned int>(nearbyWp.size())){
//                                                        attempts++;
//                                                        pool = nearbyWp[genNonCommute()];
//                                                        if (attempts == static_cast<unsigned int>(nearbyWp.size())){
//                                                                count++;
//                                                        }
//                                                }

                                                auto firstGenDraw = genNonCommute();
                                                auto genDraw = firstGenDraw;
                                                while (nearbyWp[genDraw]->CheckLimit()) {
                                                        genDraw++;
                                                        genDraw = genDraw % static_cast<int>(nearbyWp.size());

                                                        if (genDraw == firstGenDraw) {
                                                                noncom++;
                                                                genDraw = genNonCommute();
                                                                break;
                                                        }
                                                }
                                                auto pool = nearbyWp[genDraw];

                                                pool->AddMember(person);
                                                person->SetPoolId(Id::Workplace, pool->GetId());
                                        }
                                } else {
                                        // -----------------------------
                                        // this person has no employment
                                        // -----------------------------
                                        person->SetPoolId(Id::Workplace, 0);
                                }
                        }
                }
        }

        // --------------------------------------------------------------------------------
        // Redistribute pools according to distribution file
        // --------------------------------------------------------------------------------
//        std::map<std::tuple<double, unsigned int, unsigned int>, std::vector<ContactPool*>> ActualRatios;
//        std::tuple<double, unsigned int, unsigned int> unAssigned = {0.0, 0, 0};
//        for (auto workpool: nearbyWp){
//                const auto size = workpool->size();
//                for (const auto entry: distribution) {
//                        unsigned int minSize = std::get<1>(entry);
//                        unsigned int maxSize = std::get<2>(entry);
//                        if ((unsigned) (size - minSize) <= (minSize - maxSize)) {
//                                ActualRatios[entry].emplace_back(workpool);
//                        } else if (entry == distribution[distribution.size()-1]){
//                                ActualRatios[unAssigned].emplace_back(workpool);
//                        }
//                }
//        }
        cerr << "Total work people: "<< total << endl;
        cerr << "Commute work people: "<< com << endl;
        cerr << "Non-commute work people: "<< noncom << endl;


        m_logger->trace("Done populating Workplaces");
}

} // namespace geopop
