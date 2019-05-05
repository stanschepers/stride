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
 *  Copyright 2018, 2019Jan Broeckhove and Bistromatics group.
 */

#include "GeoGridHDF5Reader.h"

#include "geopop/GeoGrid.h"
#include "pop/Person.h"
#include "pop/Population.h"

#include <iostream>
#include <stdexcept>

namespace geopop {

using namespace std;
using namespace stride::ContactType;

GeoGridHDF5Reader::GeoGridHDF5Reader(std::string inputFilePath, stride::Population* pop)
    : GeoGridFileReader(std::move(inputFilePath), pop)
{
}

void GeoGridHDF5Reader::Read() { GeoGridFileReader::Read(); }

} // namespace geopop
