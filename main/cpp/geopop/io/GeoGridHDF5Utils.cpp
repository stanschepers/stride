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

#include "GeoGridHDF5Utils.h"

namespace H5Utils {

template <>
H5::CompType GetCompoundType<H5Commute>()
{
        H5::CompType commute_t(sizeof(H5Commute));
        commute_t.insertMember("to", HOFFSET(H5Commute, to), H5::PredType::NATIVE_UINT);
        commute_t.insertMember("proportion", HOFFSET(H5Commute, proportion), H5::PredType::NATIVE_DOUBLE);
        return commute_t;
}

template <>
H5::CompType GetCompoundType<H5PoolPerson>()
{
        H5::CompType pool_person_t(sizeof(H5PoolPerson));
        pool_person_t.insertMember("id", HOFFSET(H5PoolPerson, id), H5::PredType::NATIVE_UINT);
        return pool_person_t;
}

template <>
H5::CompType GetCompoundType<H5Person>()
{
}

} // namespace H5Utils
