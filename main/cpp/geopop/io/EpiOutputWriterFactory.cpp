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

#include "EpiOutputWriterFactory.h"
#include "EpiOutputHDF5Writer.h"
#include "EpiOutputJSONWriter.h"
#include "EpiOutputProtoWriter.h"
#include "EpiOutputWriter.h"
#include "util/Exception.h"

#include <iostream>

#ifdef BOOST_FOUND
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
namespace filesys = boost::filesystem;
#else
#include <filesystem>
namespace filesys = std::filesystem;
#endif

namespace geopop {

std::shared_ptr<EpiOutputWriter> EpiOutputWriterFactory::CreateEpiOutputWriter(const std::string& filename)
{
        filesys::path path(filename);

        if (path.extension().string() == ".json") {
                return std::make_shared<EpiOutputJSONWriter>(filename);
        } else if (path.extension().string() == ".proto") {
                return std::make_shared<EpiOutputProtoWriter>(filename);
        } else if (path.extension().string() == ".h5") {
                return std::make_shared<EpiOutputHDF5Writer>(filename);
        } else {
                throw stride::util::Exception("GeoGridWriterFactory::CreateWriter> Unsupported file extension: " +
                                              path.extension().string());
        }
}

} // namespace geopop
