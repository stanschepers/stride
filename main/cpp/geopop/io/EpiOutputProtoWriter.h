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

#include "EpiOutputWriter.h"
#include "geopop/Location.h"
#include "geopop/io/proto_pb/epioutput.pb.h"

#include <fstream>

// namespace proto {
// class EpiOutput;
// class EpiOutput_Location;
// class EpiOutput_Location_Coordinate;
// class EpiOutput_Location_AgeBracket;
// class EpiOutput_Location_AgeBracket_HealthStatus;
//}

namespace geopop {

/**
 * Writes a GeoGrid epi-output to a protobuf file.
 */
class EpiOutputProtoWriter : public EpiOutputWriter
{
public:
        /// Construct the EpiOutputJSONWriter.
        explicit EpiOutputProtoWriter(const std::string& filename = "");

        /// Write the provided GeoGrid epi-output to the provided JSON file. (If given)
        void Write() override;

        /// Write the provided GeoGrid epi-output to the provided ostream in JSON format. (Only for test purposes)
        void Write(std::ostream& stream);

        /// Write the epi-output of the GeoGrid to the right data format.
        void Update(GeoGrid<Epidemiologic>& geoGrid, unsigned int day) override;

private:
        proto::EpiOutput m_output;  ///< protobuf to be written to a file
        std::ofstream    m_fstream; ///< The file stream.
};

} // namespace geopop
