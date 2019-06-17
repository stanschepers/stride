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
        unsigned int Household;
        unsigned int Daycare;
        unsigned int PreSchool;
        unsigned int K12School;
        unsigned int College;
        unsigned int Workplace;
        unsigned int PrimaryCommunity;
        unsigned int SecondaryCommunity;
};

struct H5EpiOutput
{
        unsigned int age_bracket;
        unsigned int health_status;
        double       percentage;
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

template <typename T>
inline void WriteAttribute(const T& value, const std::string& name, H5::H5Object& h5Object)
{
        auto attribute(h5Object.createAttribute(name, HDF5Type(value), CreateSpace(1)));
        attribute.write(HDF5Type(value), &value);
}

template <>
inline void WriteAttribute(const std::string& value, const std::string& name, H5::H5Object& h5Object)
{
        auto attribute(h5Object.createAttribute(name, HDF5Type(value), CreateSpace(1)));
        attribute.write(HDF5Type(value), value);
}

template <typename T>
inline void ReadAttribute(T& value, const std::string& name, const H5::H5Object& h5Object)
{
        auto attribute = h5Object.openAttribute(name);
        auto dtype     = attribute.getDataType();
        attribute.read(dtype, &value);
}

template <>
inline void ReadAttribute(std::string& value, const std::string& name, const H5::H5Object& h5Object)
{
        auto attribute = h5Object.openAttribute(name);
        auto dtype     = attribute.getStrType();
        attribute.read(dtype, value);
}

} // namespace H5Utils
