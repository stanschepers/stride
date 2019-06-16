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
 *  Copyright 2019, Andrei Bondarenko.
 */

#pragma once

#include "EpiOutputWriter.h"
#include "geopop/Location.h"

#include <H5Cpp.h>

namespace geopop {

/**
 * Writes a GeoGrid epi-output to a HDF5 file.
 */
class EpiOutputHDF5Writer : public EpiOutputWriter
{
public:
        /// Construct the EpiOutputHDF5Writer.
        explicit EpiOutputHDF5Writer(const std::string& filename);

        /// Write the provided GeoGrid epi-output to the provided HDF5 file. (If given)
        void Write() override;

        /// Write the epi-output of the GeoGrid to the right data format.
        void Update(GeoGrid<Epidemiologic>& geoGrid, unsigned int day) override;

private:
        H5::H5File   m_output;         ///< HDF5 file
        unsigned int m_day_count;      ///< Used to set day_count attribute in HDF5 file
        unsigned int m_location_count; ///< Used to set location_count attribute in HDF5 file
};

} // namespace geopop
