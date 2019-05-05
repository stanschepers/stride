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
 *  Copyright 2018, Jan Broeckhove and Bistromatics group.
 */

#pragma once

#include "GeoGridFileReader.h"
#include "contact/ContactPool.h"
#include "contact/ContactType.h"
#include "geopop/Location.h"

#include <memory>
#include <set>

namespace geopop {

/**
 * An implementation of the GeoGridReader using HDF5.
 * This class is used to read a GeoGrid from an HDF5 file.
 */
class GeoGridHDF5Reader : public GeoGridFileReader
{
public:
        /// Construct the GeoGridJSONReader with the istream which contains the Protobuf info.
        GeoGridHDF5Reader(std::string inputFilePath, stride::Population* pop);

        /// No copy constructor.
        GeoGridHDF5Reader(const GeoGridHDF5Reader&) = delete;

        /// No copy assignement.
        GeoGridHDF5Reader operator=(const GeoGridHDF5Reader&) = delete;

        /// Actually perform the read and return the GeoGrid.
        void Read() override;

};

} // namespace geopop