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
 *  Copyright 2018, 2019, Jan Broeckhove, Laurens Van Damme and Bistromatics group.
 */

#pragma once

#include "contact/ContactType.h"
#include "contact/IdSubscriptArray.h"
#include "geopop/Coordinate.h"
#include "geopop/Epidemiologic.h"
#include "util/SegmentedVector.h"

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <map>
#include <stdexcept>

namespace stride {
class ContactPool;
}

namespace geopop {

/**
 * Location for use within the GeoGrid, contains Coordinate and meta data of that coordinate.
 */
template <class Content>
class Location
{
public:
        /// Parametrized constructor with population count.
        Location<Content>(unsigned int id, unsigned int province, std::shared_ptr<Content> content, Coordinate coordinate = Coordinate(0.0, 0.0), std::string name = "");
        /// Specialised contructor for Epidemiologic as template
        template <typename = std::enable_if<std::is_same<Content, Epidemiologic>::value>>
        Location<Content>(unsigned int id, unsigned int province, Coordinate coordinate = Coordinate(0.0, 0.0),
                std::string name = "", unsigned int popCount = 0U, unsigned int householdType = 0U, double youngOldFraction = 1.0)
                : m_content(std::make_shared<Content>(this, popCount, householdType, youngOldFraction)), m_coordinate(coordinate), m_id(id), m_name(move(name)), m_province(province)
        {
        }

        /// Perform a full comparison with the other location.
        bool operator==(const Location<Content>& other) const;

        /// Gets the Coordinate of this Location.
        const Coordinate GetCoordinate() const { return m_coordinate; }

        /// Gets ID of this Location.
        unsigned int GetID() const { return m_id; }

        /// Gets the name.
        std::string GetName() const { return m_name; }

        /// Gets the province.
        unsigned int GetProvince() const { return m_province; }

        /// Sets the Coordinate of this Location.
        void SetCoordinate(const Coordinate& coordinate) { m_coordinate = coordinate; }

        /// Gets the content
        std::shared_ptr<Content> GetContent() const { return m_content; }

        /// Get the latitude or longitude from the coordinate of the location
        template<unsigned int T>
        double get() const {
                if (T < 2) {
                        return m_coordinate.get<T>();
                } else {
                        throw std::out_of_range("index");
                }
        }

private:
        std::shared_ptr<Content> m_content;  ///< Content of the Location
        Coordinate   m_coordinate;           ///< Coordinate of the Location.
        unsigned int m_id = 0U;              ///< Id.
        std::string  m_name;                 ///< Name.
        unsigned int m_province;             ///< Province id.
};

} // namespace geopop

#include "Location.cpp"