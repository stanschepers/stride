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
 *  Copyright 2019, Andrei Bondarenko.
 */

#pragma once

#include "EpiOutputReader.h"

#include <H5Cpp.h>
#include <memory>

namespace visualization {

class EpiOutputReader;

/**
 * An implementation of the EpiOutputReader using HDF5.
 * This class is used to read a epi-output from a HDF5 file.
 */
class EpiOutputHDF5Reader : public EpiOutputReader
{
public:
        /// Construct the EpiOutputHDF5Reader with the istream which contains the HDF5.
        EpiOutputHDF5Reader(const std::string& filename, geopop::GeoGrid<EpiOutput>* epiOutput);

        /// No copy constructor.
        EpiOutputHDF5Reader(const EpiOutputHDF5Reader&) = delete;

        /// No copy assignement.
        EpiOutputHDF5Reader operator=(const EpiOutputHDF5Reader&) = delete;

        /// Actually perform the read and return the EpiOutput.
        void Read() override;
private:
        void ReadDay(const H5::Group& day, geopop::GeoGrid<EpiOutput>& grid);
private:
        H5::H5File m_input; ///< HDF5 file
};

} // namespace visualization
