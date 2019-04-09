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
 *  Copyright 2019, Jan Broeckhove and Bistromatics group.
 */

#include <istream>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

namespace visualization {

/**
 * An abstract base class for creating a map with the epi-output that was read from a file, can be implemented
 * using multiple file types (proto, hdf5 and json are currently implemented)
 */
class GeoGridReader
{
public:
        /// Parametrized constructor.
        EpiOutputReader(std::unique_ptr<std::istream> inputStream);

        /// No copy constructor.
        EpiOutputReader(const EpiOutputReader&) = delete;

        /// No copy assignement.
        EpiOutputReader operator=(const EpiOutputReader&) = delete;

        /// Default destructor.
        virtual ~EpiOutputReader() = default;

        /// Perform the actual read and return the created GeoGrid.
        virtual void Read() = 0;

protected:
        std::unique_ptr<std::istream> m_inputStream; ///< File to read.
};

} // namespace visualization