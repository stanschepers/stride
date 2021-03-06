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

#pragma once

#include "contact/ContactType.h"

#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "pop/Population.h"
#include "util/RnMan.h"
#include "util/LogUtils.h"

#include <spdlog/logger.h>

namespace geopop {

template <class LocationContent>
class GeoGrid;

class Epidemiologic;

/**
 * Generator uses geo & pop data to construct ContactPools in the GeoGrid.
 */
template <typename stride::ContactType::Id ID>
class Generator
{
public:
        /// Constructor with random number manager and logger.
        explicit Generator(stride::util::RnMan rnMan, std::shared_ptr<spdlog::logger> logger = nullptr)
                : m_rn_man(std::move(rnMan)), m_logger(std::move(logger))
        {
                if (!m_logger)
                        m_logger = stride::util::LogUtils::CreateNullLogger();
        }

        /// Default is OK.
        ~Generator() = default;

        /// Generate ContactPools for ContactType::Id as sepcified by data in GeoGridConfig.
        void Apply(GeoGrid<Epidemiologic>&, const GeoGridConfig&) {}

        /// Create a given number ContactPools in the GeoGrid.
        void AddPools(Location<Epidemiologic>& loc, stride::Population* pop, const GeoGridConfig& ggConfig,
                 unsigned int limit = std::numeric_limits<unsigned int>::infinity())
        {
                auto& poolSys = pop->RefPoolSys();
                for (auto i = 0U; i < ggConfig.pools[ID]; ++i) {
                        const auto p = poolSys.CreateContactPool(ID, limit);
                        loc.GetContent()->RegisterPool<ID>(p);
                }
        }

protected:
        stride::util::RnMan             m_rn_man; ///< RnManager used by generators.
        std::shared_ptr<spdlog::logger> m_logger; ///< Logger used by generators.
};

// ---------------------------------------------------------------
// Declare specializations (implementation in separate .cpp files).
// ---------------------------------------------------------------

// Apply functions
template<>
void Generator<stride::ContactType::Id::Daycare>::Apply(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& ggConfig);

template<>
void Generator<stride::ContactType::Id::PreSchool>::Apply(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& ggConfig);

template<>
void Generator<stride::ContactType::Id::K12School>::Apply(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& ggConfig);

template<>
void Generator<stride::ContactType::Id::College>::Apply(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& ggConfig);

template<>
void Generator<stride::ContactType::Id::Workplace>::Apply(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& ggConfig);

template<>
void Generator<stride::ContactType::Id::Household>::Apply(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& ggConfig);

template<>
void Generator<stride::ContactType::Id::PrimaryCommunity>::Apply(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& ggConfig);

template<>
void Generator<stride::ContactType::Id::SecondaryCommunity>::Apply(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& ggConfig);

// AddPools functions
template<>
void Generator<stride::ContactType::Id::Workplace>::AddPools(Location<Epidemiologic>& loc, stride::Population* pop, const GeoGridConfig& ggConfig, unsigned int limit);

// ---------------------------------------------------------------
// Shorthand definitions.
// ---------------------------------------------------------------
using DaycareGenerator = Generator<stride::ContactType::Id::Daycare>;
using PreSchoolGenerator = Generator<stride::ContactType::Id::PreSchool>;
using K12SchoolGenerator = Generator<stride::ContactType::Id::K12School>;
using CollegeGenerator = Generator<stride::ContactType::Id::College>;
using WorkplaceGenerator = Generator<stride::ContactType::Id::Workplace>;
using HouseholdGenerator = Generator<stride::ContactType::Id::Household>;
using PrimaryCommunityGenerator = Generator<stride::ContactType::Id::PrimaryCommunity>;
using SecondaryCommunityGenerator = Generator<stride::ContactType::Id::SecondaryCommunity>;

} // namespace geopop
