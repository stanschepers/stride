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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#pragma once

#include "CitiesReader.h"
#include "CommutesReader.h"
#include "HouseholdReader.h"

#include <istream>
#include <memory>
#include <string>

#ifdef BOOST_FOUND
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
namespace filesys = boost::filesystem;
#else
#include <filesystem>
namespace filesys = std::filesystem;
#endif

namespace gengeopop {

/**
 * A Factory for creating the correct implementation of the different Readers which
 * parse the information needed to construct a GeoGrid, based on the file extension.
 * Currently only CSV Readers are implemented.
 */
class ReaderFactory
{
public:
        /// Create a CitiesReader based on an absolute Filesystem Path pointing to the input file.
        std::shared_ptr<CitiesReader> CreateCitiesReader(const filesys::path& path);

        /// Create a CitiesReader based on the filename of the input file, relative to the data directory.
        std::shared_ptr<CitiesReader> CreateCitiesReader(const std::string& filename);

        /// Create a CommutesReader based on an absolute Filesystem Path pointing to the input file.
        std::shared_ptr<CommutesReader> CreateCommutesReader(const std::string& filename);

        /// Create a CommutesReader based on the filename of the input file, relative to the data directory.
        std::shared_ptr<CommutesReader> CreateCommutesReader(const filesys::path& path);

        /// Create a HouseholdReader based on the filename of the input file, relative to the data directory.
        std::shared_ptr<HouseholdReader> CreateHouseholdReader(const std::string& filename);

        /// Create a HouseholdReader based on an absolute Filesystem Path pointing to the input file.
        std::shared_ptr<HouseholdReader> CreateHouseholdReader(const filesys::path& path);

private:
        /// Create an istream based on the provided Filesystem Path.
        std::unique_ptr<std::istream> OpenFile(const filesys::path& path) const;
};

} // namespace gengeopop