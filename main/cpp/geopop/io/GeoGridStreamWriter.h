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

class GeoGrid;

/**
 * An interface for writing the GeoGrid to an output stream, can be implemented with multiple file types.
 * Protobuf and json are currently implemented.
 */
class GeoGridStreamWriter : public GeoGridWriter
{
public:
        /// GeoGridStreamWriter cannot be instantiated without stream
        GeoGridStreamWriter() = delete;

        /// Construct GeoGridStreamWriter referencing given stream
        explicit GeoGridStreamWriter(std::shared_ptr<std::ostream> stream) : m_stream(std::move(stream)) {};

        /// Default destructor.
        ~GeoGridStreamWriter() override = default;

        /// Write the GeoGrid to ostream.
        void Write(GeoGrid& geoGrid) override = 0;

        /// Return reference to output stream
        std::shared_ptr<std::ostream> StreamRef() { return m_stream; }

protected:
        std::shared_ptr<std::ostream> m_stream;
};

} // namespace geopop
