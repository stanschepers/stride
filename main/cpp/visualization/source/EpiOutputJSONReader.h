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

#include "EpiOutputReader.h"

#include <memory>

namespace visualization {

class EpiOutputReader;

/**
 * An implementation of the EpiOutputReader using JSON.
 * This class is used to read a epi-output from a JSON file.
 */
class EpiOutputJSONReader : public EpiOutputReader
{
public:
        /// Construct the EpiOutputJSONReader with the istream which contains the JSON.
        EpiOutputJSONReader(std::unique_ptr<std::istream> inputStream, geopop::GeoGrid<EpiOutput>* epiOutput);

        /// No copy constructor.
        EpiOutputJSONReader(const EpiOutputJSONReader&) = delete;

        /// No copy assignement.
        EpiOutputJSONReader operator=(const EpiOutputJSONReader&) = delete;

        /// Actually perform the read and return the EpiOutput.
        void Read() override;

private:
        std::unique_ptr<std::istream> m_inputStream; ///< File to read.
};

} // namespace visualization
