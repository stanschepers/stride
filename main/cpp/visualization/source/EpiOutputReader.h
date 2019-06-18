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

#include "geopop/GeoGrid.h"

#include <istream>
#include <memory>
#include <tuple>
#include <vector>

#include "visualization/source/EpiOutput.h"

namespace visualization {

/**
 * An abstract base class for creating a GeoGrid with the epi-output that was read from a file, can be implemented
 * using multiple file types (proto, hdf5 and json are currently implemented)
 */
class EpiOutputReader
{
public:
        /// Parametrized constructor.
        EpiOutputReader(geopop::GeoGrid<EpiOutput>* epiOutput);

        /// No copy constructor.
        EpiOutputReader(const EpiOutputReader&) = delete;

        /// No copy assignement.
        EpiOutputReader& operator=(const EpiOutputReader&) = delete;

        /// Default destructor.
        virtual ~EpiOutputReader() = default;

        /// Perform the actual read and return the created EpiOutput.
        virtual void Read() = 0;

protected:
        geopop::GeoGrid<EpiOutput>* m_epiOutput;     ///< Resulting epi-output
};

} // namespace visualization