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

#include "GeoGridFileWriter.h"
#include "geopop/Location.h"
#include "util/Exception.h"

#include <H5Cpp.h>
#include <set>

namespace stride {
class ContactPool;
class Person;
} // namespace stride

namespace geopop {
/**
 * An implementation of the GeoGridFileWriter using HDF5
 * This class is used to write a GeoGrid to an HDF5 file
 */
class GeoGridHDF5Writer : public GeoGridFileWriter
{
public:
        /// GeoGridProtoWriter cannot be instantiated without providing a file name.
        GeoGridHDF5Writer() = delete;

        /// Construct GeoGridHDF5Writer for provided file name
        explicit GeoGridHDF5Writer(std::string fileName);

        /// Default destructor.
        ~GeoGridHDF5Writer() override = default;

        /// Write the GeoGrid to the file in HDF5 format.
        void Write(GeoGrid& geoGrid) override;

private:
        void WriteLocation(geopop::Location& loc, H5::Group& locsGroup);

        void WriteCommutes(const std::vector<std::pair<Location*, double>>& commutes, H5::Group& locGroup);

        void WriteContactPool(const stride::ContactPool& pool, stride::ContactType::Id type, H5::Group& cpGroup);

        void WritePeople(H5::Group& rootGroup);

        template <typename T>
        void WriteAttribute(T value, const std::string& name, H5::H5Object& h5Object);

        static H5::DataSpace CreateSpace(hsize_t size);

        template <typename T>
        static inline auto HDF5Type(const T&)
        {
                throw stride::util::Exception("Tried to write unsupported type to HDF5.");
        }

        template <>
        inline auto HDF5Type(const int&)
        {
                return H5::PredType::NATIVE_INT;
        }

        template <>
        inline auto HDF5Type(const unsigned int&)
        {
                return H5::PredType::NATIVE_UINT;
        }

        template <>
        inline auto HDF5Type(const long&)
        {
                return H5::PredType::NATIVE_LONG;
        }

        template <>
        inline auto HDF5Type(const unsigned long&)
        {
                return H5::PredType::NATIVE_ULONG;
        }

        template <>
        inline auto HDF5Type(const float&)
        {
                return H5::PredType::NATIVE_FLOAT;
        }

        template <>
        inline auto HDF5Type(const double&)
        {
                return H5::PredType::NATIVE_DOUBLE;
        }

        template <>
        inline auto HDF5Type(const std::string& value)
        {
                auto str_length = value.length();
                return H5::StrType(H5::PredType::C_S1, str_length+1);
        }

private:
        std::set<stride::Person*> m_persons_found; ///< The persons found when looping over the ContactPools.
};

} // namespace geopop
