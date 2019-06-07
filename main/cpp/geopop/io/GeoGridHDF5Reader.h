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

#include "GeoGridFileReader.h"
#include "GeoGridHDF5Utils.h"
#include "contact/ContactPool.h"
#include "contact/ContactType.h"
#include "geopop/Location.h"

#include <memory>

namespace geopop {

/**
 * An implementation of the GeoGridFileReader using HDF5.
 * This class is used to read a GeoGrid from an HDF5 file.
 */
class GeoGridHDF5Reader : public GeoGridFileReader
{
public:
        /// Construct the GeoGridHDF5Reader with the string which contains the location of the input file.
        GeoGridHDF5Reader(std::string inputFilePath, stride::Population* pop);

        /// No copy constructor.
        GeoGridHDF5Reader(const GeoGridHDF5Reader&) = delete;

        /// No copy assignement.
        GeoGridHDF5Reader operator=(const GeoGridHDF5Reader&) = delete;

        /// Actually perform the read.
        void Read() override;

private:
        /// People
        void ReadPeople(const H5::DataSet& people);
        void ReadPerson(const H5Utils::H5Person& person);

        /// Locations
        void ReadLocations(const H5::Group& locations, GeoGrid& grid);
        void ReadLocation(const H5::Group& location, GeoGrid& grid);
        void ReadContactPool(const H5::DataSet& pool, const std::shared_ptr<Location>& location);

};

} // namespace geopop
