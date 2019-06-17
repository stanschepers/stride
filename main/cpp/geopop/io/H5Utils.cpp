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

#include "H5Utils.h"

namespace H5Utils {

using namespace std;
using namespace H5;

template <>
CompType GetCompoundType<H5Commute>()
{
        CompType commute_t(sizeof(H5Commute));
        commute_t.insertMember("to", HOFFSET(H5Commute, to), PredType::NATIVE_UINT);
        commute_t.insertMember("proportion", HOFFSET(H5Commute, proportion), PredType::NATIVE_DOUBLE);
        return commute_t;
}

template <>
CompType GetCompoundType<H5PoolPerson>()
{
        CompType pool_person_t(sizeof(H5PoolPerson));
        pool_person_t.insertMember("id", HOFFSET(H5PoolPerson, id), PredType::NATIVE_UINT);
        return pool_person_t;
}

template <>
CompType GetCompoundType<H5Person>()
{
        CompType person_t(sizeof(H5Person));
        person_t.insertMember("id", HOFFSET(H5Person, id), PredType::NATIVE_UINT);
        person_t.insertMember("age", HOFFSET(H5Person, age), PredType::NATIVE_FLOAT);
        person_t.insertMember("Household", HOFFSET(H5Person, Household), PredType::NATIVE_UINT);
        person_t.insertMember("K12School", HOFFSET(H5Person, K12School), PredType::NATIVE_UINT);
        person_t.insertMember("College", HOFFSET(H5Person, College), PredType::NATIVE_UINT);
        person_t.insertMember("Workplace", HOFFSET(H5Person, Workplace), PredType::NATIVE_UINT);
        person_t.insertMember("PrimaryCommunity", HOFFSET(H5Person, PrimaryCommunity), PredType::NATIVE_UINT);
        person_t.insertMember("SecondaryCommunity", HOFFSET(H5Person, SecondaryCommunity), PredType::NATIVE_UINT);
        return person_t;
}

template <>
CompType GetCompoundType<H5EpiOutput>()
{
        CompType epi_t(sizeof(H5EpiOutput));
        epi_t.insertMember("age_bracket", HOFFSET(H5EpiOutput, age_bracket), PredType::NATIVE_UINT);
        epi_t.insertMember("health_status", HOFFSET(H5EpiOutput, health_status), PredType::NATIVE_UINT);
        epi_t.insertMember("percentage", HOFFSET(H5EpiOutput, percentage), PredType::NATIVE_DOUBLE);
        return epi_t;
}

DataSpace CreateSpace(hsize_t size)
{
        hsize_t dim[] = {size};
        return DataSpace(1, dim);
}

H5::PredType HDF5Type(const int&) { return PredType::NATIVE_INT; }

H5::PredType HDF5Type(const unsigned int&) { return PredType::NATIVE_UINT; }

H5::PredType HDF5Type(const long&) { return PredType::NATIVE_LONG; }

H5::PredType HDF5Type(const unsigned long&) { return PredType::NATIVE_ULONG; }

H5::PredType HDF5Type(const float&) { return PredType::NATIVE_FLOAT; }

H5::PredType HDF5Type(const double&) { return PredType::NATIVE_DOUBLE; }

H5::StrType HDF5Type(const string& value)
{
        auto str_length = value.length();
        return StrType(PredType::C_S1, str_length);
}

} // namespace H5Utils
