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

#include "PreSchoolPopulator.h"

#include "contact/AgeBrackets.h"
#include "contact/ContactPool.h"
#include "geopop/GeoGrid.h"
#include "geopop/HouseholdCenter.h"
#include "geopop/PreSchoolCenter.h"
#include "geopop/Location.h"
#include "pop/Person.h"

namespace geopop {

    using namespace std;
    using namespace stride;
    using namespace stride::ContactType;

void PreSchoolPopulator::Apply(geopop::GeoGrid &geoGrid, const geopop::GeoGridConfig&)
{
        m_logger->trace("Starting to populate Daycare's");

        set<ContactPool*> found;
        unsigned int      pupils = 0;

        // for every location
        for (const auto& loc : geoGrid) {
                if (loc->GetPopCount() == 0) {
                        continue;
                }

                // 1. find all schools in an area of 10-k*10 km
                const vector<ContactPool*>& classes = GetNearbyPools(Id::PreSchool, geoGrid, *loc);

                auto dist = m_rn_man.GetUniformIntGenerator(0, static_cast<int>(classes.size()), 0U);

                // 2. for every student assign a class
                for (const auto& hhCenter : loc->RefCenters(Id::Household)) {
                        ContactPool* const contactPool = (*hhCenter)[0];
                        found.insert(contactPool);
                        for (Person* p : *contactPool) {
                                if (AgeBrackets::PreSchool::HasAge(p->GetAge())) {
                                        auto& c = classes[dist()];
                                        c->AddMember(p);
                                        p->SetPoolId(Id::PreSchool, c->GetId());
                                        pupils++;
                                }
                        }
                }
        }

        m_logger->debug("Number of pupils in preschools: {}", pupils);
        m_logger->debug("Number of different classes: {}", found.size());
        m_logger->trace("Done populating PreSchools");
}

} // namespace geopop
