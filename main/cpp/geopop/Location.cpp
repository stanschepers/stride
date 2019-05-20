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

//#include "Location.h"
#include "contact/ContactPool.h"
#include "contact/ContactType.h"
#include "disease/Health.h"
#include "pop/Person.h"
#include "util/Exception.h"

#include <cmath>

namespace geopop {

using namespace std;
using namespace stride::ContactType;

template <class Content>
Location<Content>::Location(unsigned int id, unsigned int province, std::shared_ptr<Content> content, Coordinate coordinate, string name)
    : m_content(content), m_coordinate(coordinate), m_id(id), m_name(move(name)), m_province(province)
{
}

template <class Content>
bool Location<Content>::operator==(const Location<Content>& other) const
{
        using boost::geometry::get;

        return GetID() == other.GetID() && get<0>(GetCoordinate()) == get<0>(other.GetCoordinate()) &&
                  get<1>(GetCoordinate()) == get<1>(other.GetCoordinate()) && GetName() == other.GetName() &&
                  GetProvince() == other.GetProvince() && m_content->operator==(*other.GetContent());
}

} // namespace geopop
