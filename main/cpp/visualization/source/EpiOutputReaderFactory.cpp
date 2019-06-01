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

#include "EpiOutputReaderFactory.h"

#include "EpiOutputJSONReader.h"
#include "EpiOutputProtoReader.h"
//#include "EpiOutputHDF5Reader.h"

#include <fstream>
#include <memory>

#ifdef BOOST_FOUND
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
namespace filesys = boost::filesystem;
#else
#include <filesystem>
namespace filesys = std::filesystem;
#endif

namespace visualization {

std::shared_ptr<EpiOutputReader> EpiOutputReaderFactory::CreateReader(const std::string& filename, geopop::GeoGrid<EpiOutput>* epiOutput) const
{
        const filesys::path path(filename);
        if (!filesys::exists(path)) {
                throw stride::util::Exception("EpiOutputReaderFactory::CreateReader> File not found: " + path.string());
        }
//        std::string path = filename;

        if (path.extension().string() == ".json") {  //if (path.substr(path.size() - 5) == ".json") {
                return std::make_shared<EpiOutputJSONReader>(std::make_unique<std::ifstream>(path.string()), epiOutput);
        }
        else if (path.extension().string() == ".proto") {  //if (path.substr(path.size() - 6) == ".proto") {
                return std::make_shared<EpiOutputProtoReader>(std::make_unique<std::ifstream>(path.string()), epiOutput);
        } /*else
                if (path == ".hfd5") {
                return std::make_shared<EpiOutputHDF5Reader>(std::make_unique<std::ifstream>(path.string()));
        } */
        else {
                throw stride::util::Exception("EpiOutputReaderFactory::CreateReader> Unsupported file extension: " + path.string());
        }
}

} // namespace visualization
