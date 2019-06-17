#include <utility>

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

#include <memory>
#include <ostream>

#include "GeoGridWriter.h"

namespace geopop {

template <class LocationContent>
class GeoGrid;

/**
 * An interface for writing the GeoGrid to a filelocated at inputFilePath,
 * can be implemented with multiple file types. HDF5 is currently implemented.
 */
class GeoGridFileWriter : public GeoGridWriter
{
public:
        /// GeoGridFileWriter cannot be instantiated without filename
        GeoGridFileWriter() = delete;

        /// Construct GeoGridFileWriter referencing given filename
        explicit GeoGridFileWriter(std::string  fileName) : m_file_name(std::move(fileName)) {};

        /// Default destructor.
        ~GeoGridFileWriter() override = default;

        /// Write the GeoGrid to file.
        void Write(GeoGrid<Epidemiologic>& geoGrid) override = 0;

        /// Return file name
        std::string FileName() { return m_file_name; }

protected:
        std::string m_file_name;
};

} // namespace geopop
