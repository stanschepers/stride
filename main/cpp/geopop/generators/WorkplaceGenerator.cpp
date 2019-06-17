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

#include "Generator.h"

#include "util/Assert.h"
#include "util/RnMan.h"

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;

template <>
void Generator<stride::ContactType::Id::Workplace>::Apply(GeoGrid& geoGrid, const GeoGridConfig& ggConfig)
{
        // 1. active people count and the commuting people count are given
        // 2. count the workplaces, either by poolparams or if given: the work distribution
        // 3. count the working people at each location = #residents + #incoming commuters - #outgoing commuters
        // 4. use that information for the distribution
        // 5. assign each workplaces to a location

        const auto EmployeeCount = ggConfig.info.popcount_workplace;
        auto       WorkplacesCount =
            static_cast<unsigned int>(ceil(EmployeeCount / static_cast<double>(ggConfig.people[Id::Workplace])));

        // if distribution file is present, make accurate estimation of WorkplacesCount
        const auto     distribution = ggConfig.param.work_distribution;
        vector<double> workWeights;
        if (!distribution.empty()) {
                double PoolSize = 0;
                for (auto entry : distribution) {
                        double       ratio   = std::get<0>(entry);
                        unsigned int minSize = std::get<1>(entry);
                        unsigned int maxSize = std::get<2>(entry);

                        workWeights.emplace_back(ratio);
                        double avg = (minSize + maxSize) / 2.0;
                        PoolSize += avg * ratio;
                }
                const auto NewWorkplacesCount = static_cast<unsigned int>(ceil(EmployeeCount / PoolSize));
                m_logger->trace("Number of workplaces through distribution file: " + to_string(NewWorkplacesCount) +
                                ", instead of: " + to_string(WorkplacesCount));
                WorkplacesCount = NewWorkplacesCount;
        }

        // = for each location #residents + #incoming commuting people - #outgoing commuting people
        vector<double> weights;
        for (const auto& loc : geoGrid) {
                const double ActivePeopleCount =
                    (loc->GetPopCount() + loc->GetIncomingCommuteCount(ggConfig.param.fraction_workplace_commuters) -
                     loc->GetOutgoingCommuteCount(ggConfig.param.fraction_workplace_commuters) *
                         ggConfig.param.participation_workplace);

                const double weight = ActivePeopleCount / EmployeeCount;
                AssertThrow(weight >= 0 && weight <= 1 && !std::isnan(weight), "Invalid weight: " + to_string(weight),
                            m_logger);
                weights.push_back(weight);
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        const auto dist = m_rn_man.GetDiscreteGenerator(weights, 0U);
        auto       pop  = geoGrid.GetPopulation();

        unsigned int entryIndex = 0;
        int          totalSize  = 0;
        for (auto i = 0U; i < WorkplacesCount; i++) {
                const auto loc = geoGrid[dist()];

                // ----------------------------------------------
                // If there's distribution, add pools using limit
                // ----------------------------------------------
                if (!distribution.empty()) {
                        auto workClass = m_rn_man.GetDiscreteGenerator(workWeights, 0U);
                        entryIndex     = workClass();
                        auto minSize   = std::get<1>(distribution[entryIndex]);
                        auto maxSize   = std::get<2>(distribution[entryIndex]);

                        auto         genSize = m_rn_man.GetUniformIntGenerator(minSize, maxSize + 1U); // [a, b[
                        unsigned int s       = genSize();
                        totalSize += s;
                        AddPools(*loc, pop, ggConfig, s);
                } else {
                        AddPools(*loc, pop, ggConfig);
                }
        }

        if (!distribution.empty()) {
                m_logger->trace("Total generated size in workplaces: " + to_string(totalSize));
                m_logger->trace("Total number of employees: " + to_string(EmployeeCount));
        }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
template <>
void Generator<stride::ContactType::Id::Workplace>::AddPools(Location& loc, stride::Population* pop,
                                                             const GeoGridConfig& ggConfig, unsigned int limit)
{
        auto&      poolSys = pop->RefPoolSys();
        const auto p       = poolSys.CreateContactPool(stride::ContactType::Id::Workplace, limit);
        loc.RegisterPool<stride::ContactType::Id::Workplace>(p);
}
#pragma clang diagnostic pop

} // namespace geopop
