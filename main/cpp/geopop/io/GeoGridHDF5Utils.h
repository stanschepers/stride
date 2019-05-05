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

#include <H5Cpp.h>

namespace H5Utils {

struct H5Commute
{
        unsigned int to;
        double       proportion;
};

struct H5PoolPerson
{
        unsigned int id;
};

struct H5Person
{
        unsigned int id;
        float        age;
        unsigned int household_id;
        unsigned int k12School_id;
        unsigned int college_id;
        unsigned int workplace_id;
        unsigned int primaryCommunity_id;
        unsigned int secondaryCommunity_id;
};

template <typename T>
H5::CompType GetCompoundType();

H5::DataSpace CreateSpace(hsize_t size);

H5::PredType HDF5Type(const int&);

H5::PredType HDF5Type(const unsigned int&);

H5::PredType HDF5Type(const long&);

H5::PredType HDF5Type(const unsigned long&);

H5::PredType HDF5Type(const float&);

H5::PredType HDF5Type(const double&);

H5::StrType HDF5Type(const std::string& value);

} // namespace H5Utils
