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

#pragma once

#include "contact/ContactType.h"
#include "contact/IdSubscriptArray.h"
#include "util/SegmentedVector.h"

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <map>

namespace stride {
    class ContactPool;
}

namespace geopop {

template <class Content>
class Location;

/**
 * Epidemiologic for use within the GeoGrid, contains index to ContactPools and commutes.
 */
    class Epidemiologic {
    public:
        /// Parametrized constructor with population count.
        explicit Epidemiologic(Location<Epidemiologic>* location, unsigned int popCount = 0U);

        /// Perform a full comparison with the other Epidemiologic.
        bool operator==(const Epidemiologic &other) const;

        /// Adds a Epidemiologic and a proportion to the incoming commute vector.
        /// I.e. fraction of commuting population at otherEpidemiologic commuting to this Epidemiologic.
        void AddIncomingCommute(std::shared_ptr <Epidemiologic> otherEpidemiologic, double fraction);

        /// Adds a Epidemiologic and a fraction to the outgoing commute vector.
        /// I.e. fraction of commuting population at this Epidemiologic commuting to otherEpidemiologic.
        void AddOutgoingCommute(std::shared_ptr <Epidemiologic> otherEpidemiologic, double fraction);

        /// Calculates number of incomming commuters, given the fraction of the population that commutes.
        int GetIncomingCommuteCount(double fractionCommuters) const;

        /// Gets the number of people infected in the contactpools at this Epidemiologic.
        unsigned int GetInfectedCount() const;

        /// Calculates number of outgoing commuters, given the fraction of the population that commutes.
        unsigned int GetOutgoingCommuteCount(double fractionCommuters) const;

        /// Gets the absolute population.
        unsigned int GetPopCount() const { return m_pop_count; }

        /// Gets the amount of members in all household pools.
        unsigned int GetMemberCount() const;

        /// Get Epidemiologic's population fraction (of the total population count).
        double GetPopFraction() const;

        /// Get the ID of the location
        unsigned int GetID() const;

        /// Get the location of which this is the content
        Location<Epidemiologic>* GetLocation() const { return m_location; }

        /// Generate an epi-output map of the Household contactpools.
        std::map<std::string, std::map<std::string, double>> const GenerateEpiOutput();

        /// Set Epidemiologic's population count using its population fraction and the total population count.
        void SetPopCount(unsigned int totalPopCount);

        /// Set Epidemiologic's population fraction (of the total population count).
        void SetPopFraction(double relativePopulation);

    public:
        /// Access through const reference to ContactPools of type 'id'.
        /// \param id   ContactType::Id of pools container you want to access.
        /// \return     The requested reference.
        const stride::util::SegmentedVector<stride::ContactPool*>& CRefPools(stride::ContactType::Id id) const
        {
            return m_pool_index[id];
        }

        /// Templated version of @CRefPools for use when the type id is fixed
        /// \tparam T   ContactType::Id of pools container you want to access.
        /// \return     The requested reference.
        template <stride::ContactType::Id T>
        const stride::util::SegmentedVector<stride::ContactPool*>& CRefPools() const
        {
            return m_pool_index[T];
        }

        /// Access through reference to ContactPools of type 'id'.
        /// \param id   ContactType::Id of pools container you want to access.
        /// \return     The requested reference.
        stride::util::SegmentedVector<stride::ContactPool*>& RefPools(stride::ContactType::Id id)
        {
            return m_pool_index[id];
        }

        /// Templated version of @RefPools for use when the type id is fixed
        /// \tparam T   ContactType::Id of pools container you want to access.
        /// \return     The requested reference.
        template <stride::ContactType::Id T>
        stride::util::SegmentedVector<stride::ContactPool*>& RefPools()
        {
            return m_pool_index[T];
        }

        /// Register a ContactPool pointer in this Epidemiologic's pool system.
        /// Prior to this the pool should have been created in Population's pool system.
        void RegisterPool(stride::ContactPool* p, stride::ContactType::Id typeId)
        {
            m_pool_index[typeId].emplace_back(p);
        }

        /// Templated version of @RegisterPool
        template <stride::ContactType::Id T>
        void RegisterPool(stride::ContactPool* p)
        {
            m_pool_index[T].emplace_back(p);
        }

    public:
        /// References incoming commute Epidemiologics + fraction of commutes to that Epidemiologic.
        const std::vector<std::pair<Epidemiologic*, double>>& CRefIncomingCommutes() const { return m_inCommutes; }

        /// References outgoing commute Epidemiologics + fraction of commutes to that Epidemiologic.
        const std::vector<std::pair<Epidemiologic*, double>>& CRefOutgoingCommutes() const { return m_outCommutes; }

    private:
        unsigned int m_pop_count;             ///< Population count (number of individuals) at this Epidemiologic.
        double       m_pop_fraction;          ///< Fraction of whole population at this Epidemiologic.
        Location<Epidemiologic>* m_location;  ///< Link to the location


        /// Incomming commutes stored as pair of Epidemiologic and fraction of population at that Epidemiologic.
        std::vector<std::pair<Epidemiologic*, double>> m_inCommutes;

        ///< Outgoing commutes stored as pair of Epidemiologic and fraction of population to this this Epidemiologic.
        std::vector<std::pair<Epidemiologic*, double>> m_outCommutes;

        ///< The system holding pointers to the contactpools (for each type id) at this Epidemiologic.
        stride::ContactType::IdSubscriptArray<stride::util::SegmentedVector<stride::ContactPool*>> m_pool_index;

    };

} // namespace geopop
