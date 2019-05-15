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

#include <nlohmann/json.hpp>

namespace stride {
class ContactPool;
class Person;
} // namespace stride

namespace geopop {

/**
 * Writes a GeoGrid epi-output to a JSON file.
 */
class EpiOutputJSONWriter : public EpiOutputWriter
{
public:
        /// Construct the EpiOutputJSONWriter.
        explicit EpiOutputJSONWriter();

        /// Write the provided GeoGrid epi-output to the provided ostream in JSON format.
        void Write(std::ostream& stream) override;

        /// Write the epi-output of the GeoGrid to the right data format.
        void Update(GeoGrid<Epidemiologic>& geoGrid, unsigned int day) override;

private:
        nlohmann::json m_output;  ///< json to be written to a file
};

} // namespace geopop
