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

#include "GeoGridStreamReader.h"
#include "contact/ContactPool.h"
#include "contact/ContactType.h"
#include "geopop/Location.h"
#include <nlohmann/json.hpp>
#include <spdlog/logger.h>

namespace geopop {

template <class LocationContent>
class GeoGrid;

/**
 * An implementation of the GeoGridReader using JSON.
 * This class is used to read a GeoGrid from a JSON file.
 */
class GeoGridJSONReader : public GeoGridStreamReader
{
public:
        /// Construct the GeoGridJSONReader with the istream which contains the JSON.
        GeoGridJSONReader(std::unique_ptr<std::istream> inputStream, stride::Population* pop);

        /// No copy constructor.
        GeoGridJSONReader(const GeoGridJSONReader&) = delete;

        /// No copy assignement.
        GeoGridJSONReader operator=(const GeoGridJSONReader&) = delete;

        /// Actually perform the read and return the GeoGrid.
        void Read() override;

private:
        /// Create a Person based on the information stored in the provided JSON.
        stride::Person* ParsePerson(const nlohmann::json& person);
        /// Create a Location based on the information stored in the provided JSON.
        std::shared_ptr<Location<Epidemiologic>> ParseLocation(const nlohmann::json& location);
        /// Create a Coordinate based on the information stored in the provided JSON.
        Coordinate ParseCoordinate(const nlohmann::json& coordinate);
        /// Create a ContactPool based on the information stored in the provided JSON.
        void ParseContactPool(const std::shared_ptr<Location<Epidemiologic>>& location,
                              const nlohmann::json& contactPool, stride::ContactType::Id type);
        /// Parse value of type T
        template <typename T>
        T ParseValue(const nlohmann::json& value) const;
};

} // namespace geopop
