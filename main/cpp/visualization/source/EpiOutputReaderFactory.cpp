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

#include "EpiOutputReaderFactory.h"

#include "EpiOutputJSONReader.h"
//#include "EpiOutputProtoReader.h"
//#include "EpiOutputHDF5Reader.h"

#include <fstream>
#include <memory>

namespace visualization {

std::shared_ptr<EpiOutputReader> EpiOutputReaderFactory::CreateReader(const std::string& filename, EpiOutput& epiOutput) const
{
//        const filesys::path path(filename);
//        if (!filesys::exists(path)) {
//                throw Exception("EpiOutputReaderFactory::CreateReader> File not found: " + path.string());
//        }
        std::string path = filename;

        if (path.substr(path.size() - 5) == ".json") {
                return std::make_shared<EpiOutputJSONReader>(std::make_unique<std::ifstream>(path), epiOutput);
        } /*else
                if (path == ".proto") {
                return std::make_shared<EpiOutputProtoReader>(std::make_unique<std::ifstream>(path.string()));
        } else
                if (path == ".hfd5") {
                return std::make_shared<EpiOutputHDF5Reader>(std::make_unique<std::ifstream>(path.string()));
        } */else {
                throw "EpiOutputReaderFactory::CreateReader> Unsupported file extension: " + path;
        }
}

} // namespace visualization
