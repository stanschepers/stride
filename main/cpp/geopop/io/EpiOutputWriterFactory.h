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
#include <string>

namespace geopop {

class EpiOutputWriter;

/**
 * Factory creates the correct implementation of a EpiOutputWriter based on the filename extension.
 */
class EpiOutputWriterFactory
{
public:
        /// Create a EpiOutputWriter based on the provided extension in the filename.
        static std::shared_ptr<EpiOutputWriter> CreateEpiOutputWriter(const std::string& filename);
};

} // namespace geopop
