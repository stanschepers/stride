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

#include "GeoGridWriterFactory.h"

//#include "GeoGridJSONWriter.h"
#include "GeoGridHDF5Writer.h"
#include "GeoGridProtoWriter.h"
#include "GeoGridWriter.h"
#include "util/Exception.h"

#include <fstream>
#include <iostream>

#ifdef BOOST_FOUND
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
namespace filesys = boost::filesystem;
#else
#include <filesystem>
namespace filesys = std::filesystem;
#endif

using namespace std;

namespace geopop {

std::shared_ptr<GeoGridWriter> GeoGridWriterFactory::CreateGeoGridWriter(const string& filepath)
{
        filesys::path path(filepath);

        /*if (path.extension().string() == ".json") {
                return std::make_shared<GeoGridJSONWriter>();
        } else */
        if (path.extension().string() == ".proto") {
                return make_shared<GeoGridProtoWriter>(make_shared<ofstream>(path.string()));
        } else if (path.extension().string() == ".h5") {
                return make_shared<GeoGridHDF5Writer>(path.string());
        } else {
                throw stride::util::Exception("GeoGridWriterFactory::CreateWriter> Unsupported file extension: " +
                                              path.extension().string());
        }
}

} // namespace geopop
