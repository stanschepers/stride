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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#pragma once

#include "ContactCenter.h"

namespace gengeopop {

class GeoGrid;

/**
 * Models a Workplace as ContactCenter.
 */
class Workplace : public ContactCenter
{
public:
        /// Construct workplace with assigned ID.
        explicit Workplace(unsigned int id) : ContactCenter(id) {}

        /// See ContactCenter::Fill.
        void Fill(const std::shared_ptr<GeoGrid>& geoGrid) override
        {
                AddPool(geoGrid->CreateContactPool(stride::ContactPoolType::Id::Work));
        }

        /// See ContactCenter::GetMaxPools.
        unsigned int GetMaxPools() const override { return 1; }

        /// See ContactCenter::GetPoolSize.
        unsigned int GetPoolSize() const override { return 20; }

        /// See ContactCenter::GetType.
        std::string GetType() const override { return "Workplace"; }
};

} // namespace gengeopop