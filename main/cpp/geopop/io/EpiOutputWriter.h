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
 *  Copyright 2019, Laurens Van Damme.
 */

#pragma once

#include <memory>
#include <ostream>

namespace geopop {

template <class LocationContent>
class GeoGrid;

class Epidemiologic;

/**
 * An interface for writing the epi-output of a GeoGrid to a file, can be implemented with multiple file types.
 * JSON, Protobuf and HDF5 are currently implemented.
 */
class EpiOutputWriter
{
public:
        /// Construct the Writer.
        virtual ~EpiOutputWriter() = default;

        /// Write the epi-output to the ostream.
        virtual void Write(std::ostream& stream) = 0;

        /// Write the epi-output of the GeoGrid to the right data format.
        virtual void Update(GeoGrid<Epidemiologic>& geoGrid, unsigned int day) = 0;
};

} // namespace geopop
