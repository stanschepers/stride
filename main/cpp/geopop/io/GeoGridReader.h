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

#include <istream>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

namespace stride {
class Person;
class Population;
} // namespace stride

namespace geopop {

template <class LocationContent>
class GeoGrid;

class Epidemiologic;

/**
 * An abstract base class for creating a GeoGrid that was read from a file, can be implemented
 * using multiple file types (proto, hdf5 and json are currently implemented)
 */
class GeoGridReader
{
public:
        /// Parametrized constructor.
        explicit GeoGridReader(stride::Population* pop);

        /// No copy constructor.
        GeoGridReader(const GeoGridReader&) = delete;

        /// No copy assignment.
        GeoGridReader& operator=(const GeoGridReader&) = delete;

        /// Default destructor.
        virtual ~GeoGridReader() = default;

        /// Perform the actual read.
        virtual void Read() = 0;

protected:
        /// Add the commutes that were found to their respective Locations symmetrically.
        void AddCommutes(GeoGrid<Epidemiologic>& geoGrid);

protected:
        ///< Store the persons (id->person) that were found while loping over the ContactPools.
        std::map<unsigned int, stride::Person*> m_people;

        ///< Commutes from, to, number.
        std::vector<std::tuple<unsigned int, unsigned int, double>> m_commutes;

        stride::Population*           m_population;  ///< Population to use in the GeoGrid may be nullptr.
};

} // namespace geopop
