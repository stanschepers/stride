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

#include "DaycarePopulator.h"

#include "contact/AgeBrackets.h"
#include "geopop/GeoGrid.h"
#include "geopop/Household.h"
#include "geopop/Daycare.h"
#include "geopop/Location.h"
#include "pop/Person.h"

#include <trng/uniform_int_dist.hpp>

namespace geopop {

    using namespace std;
    using namespace stride;
    using namespace stride::ContactType;

    void DaycarePopulator::Apply(shared_ptr<GeoGrid> geoGrid, const GeoGridConfig&)
    {
        m_logger->info("Starting to populate Daycare's");

        set<ContactPool*> found;
        unsigned int      babies = 0;

        // for every location
        for (const shared_ptr<Location>& loc : *geoGrid) {
            if (loc->GetPopCount() == 0) {
                continue;
            }

            // 1. find all daycare's in an area of 10-k*10 km
            const vector<ContactPool*>& classes = GetNearbyPools<Daycare>(geoGrid, loc);

            auto dist = m_rnManager[0].variate_generator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(classes.size())));

            // 2. for every student assign a class
            for (const shared_ptr<ContactCenter>& household : loc->GetContactCentersOfType<Household>()) {
                ContactPool* contactPool = household->GetPools()[0];
                found.insert(contactPool);
                for (Person* p : *contactPool) {
                    if (AgeBrackets::Daycare::HasAge(p->GetAge())) {
                        auto& c = classes[dist()];
                        c->AddMember(p);
                        p->SetPoolId(Id::Daycare, c->GetId());
                        babies++;
                    }
                }
            }
        }

        m_logger->info("Number of babies in daycare's: {}", babies);
        m_logger->info("Number of different classes: {}", found.size());
    }

} // namespace geopop
