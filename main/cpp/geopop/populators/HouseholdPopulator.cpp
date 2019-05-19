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

#include "HouseholdPopulator.h"

#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "pop/Population.h"

namespace geopop {

using namespace std;
using namespace stride::ContactType;

void HouseholdPopulator::Apply(GeoGrid& geoGrid, const GeoGridConfig& geoGridConfig)
{
        m_logger->trace("Starting to populate Households");

        auto person_id = 0U;

        if (geoGridConfig.refHHperHHType.empty()) {
                auto hh_dist   = m_rn_man.GetUniformIntGenerator(0, static_cast<int>(geoGridConfig.refHH.ages.size()), 0U);
                auto pop       = geoGrid.GetPopulation();

                for (const shared_ptr<Location>& loc : geoGrid) {
                        for (auto& pool : loc->RefPools(Id::Household)) {
                                const auto hDraw = static_cast<unsigned int>(hh_dist());
                                for (const auto& age : geoGridConfig.refHH.ages[hDraw]) {
                                        const auto p = pop->CreatePerson(person_id++, age, pool->GetId(), 0, 0, 0, 0, 0, 0, 0);
                                        pool->AddMember(p);
                                }
                        }
                }
        }
        else {
                map<unsigned int, std::function<int()>> genPerHhType;

                auto i_gen = 0U;
                for (auto const& [type, refHH] : geoGridConfig.refHHperHHType) { // TODO make less generators
                        genPerHhType[type] = m_rn_man.GetUniformIntGenerator(0,
                                static_cast<int>(refHH.ages.size()), 0U);
                        cout << type << endl;
                        i_gen++;
                }
                auto pop       = geoGrid.GetPopulation();

                for (const shared_ptr<Location>& loc : geoGrid) {
                        auto hh_dist = genPerHhType.at(loc->GetHouseHoldType());
                        for (auto& pool : loc->RefPools(Id::Household)) {
                                const auto hDraw = static_cast<unsigned int>(hh_dist());
                                for (const auto& age : geoGridConfig.refHHperHHType.at(loc->GetHouseHoldType()).ages[hDraw]) {
                                        const auto p = pop->CreatePerson(person_id++, age, pool->GetId(), 0, 0, 0, 0, 0, 0, 0);
                                        pool->AddMember(p);
                                }
                        }
                }
        }

        m_logger->trace("Done populating Households");
}

} // namespace geopop
