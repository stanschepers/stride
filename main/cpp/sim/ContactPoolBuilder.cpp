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
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Implementation for the ContactPoolBuilder class.
 */

#include "ContactPoolBuilder.h"

#include "core/ContactPoolSys.h"
#include "pop/Population.h"

namespace stride {

using namespace boost::property_tree;
using namespace std;

ContactPoolBuilder::ContactPoolBuilder(std::shared_ptr<spdlog::logger> logger) : m_stride_logger(std::move(logger))
{
        assert(m_stride_logger && "ContactPoolBuilder::ContactPoolBuilder> Nullptr for logger not acceptable!");
}

void ContactPoolBuilder::Build(ContactPoolSys& pool_sys, const Population& population)
{
        using namespace ContactPoolType;
        m_stride_logger->trace("Starting ContactPoolBuilder::Build.");

        // --------------------------------------------------------------
        // Determine number of contact pools.
        // --------------------------------------------------------------
        std::array<unsigned int, ContactPoolType::NumOfTypes()> max_ids = {0U};
        for (const auto& p : population) {
                for (Id typ : IdRange) {
                        max_ids[ToSizeT(typ)] = max(max_ids[ToSizeT(typ)], p.GetContactPoolId(typ));
                }
        }

        // --------------------------------------------------------------
        // Keep separate id counter to provide a unique id for every contactpool.
        // Start at 1 (see next item for pool_id==0).
        // --------------------------------------------------------------
        unsigned int c_id = 1;
        for (Id typ : IdRange) {
                for (size_t i = 0; i <= max_ids[ToSizeT(typ)]; i++) {
                        pool_sys[ToSizeT(typ)].emplace_back(ContactPool(c_id, typ));
                        c_id++;
                }
        }

        // --------------------------------------------------------------
        // Insert persons (pointers) in their contactpools.
        // Having contactpool id '0' means "not present in any pool of that type".
        // --------------------------------------------------------------
        for (auto& p : population) {
                for (Id typ : IdRange) {
                        const auto pool_id = p.GetContactPoolId(typ);
                        if (pool_id > 0) {
                                pool_sys[ToSizeT(typ)][pool_id].AddMember(&p);
                        }
                }
        }

        // --------------------------------------------------------------
        // Done.
        // --------------------------------------------------------------
        m_stride_logger->trace("Finished ContactPoolBuilder::Build.");
}

} // namespace stride