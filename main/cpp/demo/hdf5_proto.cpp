//
// Created by Andrei Bondarenko on 2019-03-20.
//

#include <H5Cpp.h>
#include <iostream>
#include <string>

using namespace H5;

namespace hdf5demo {

    void run(){
        const H5std_string	FILE_NAME("geogrid.h5");
        const int	        PERSONS_PROP_N = 6;
        const int	        COMMUTE_PROP_N = 2;
        const int	        CONTACT_POOLS_PROP_N = 3;
        const int	        DUMMY_Y = 5;
        const int	        RANK = 2;

        try
        {
            // Turn off the auto-printing when failure occurs so that we can
            // handle the errors appropriately
            Exception::dontPrint();

            // Create a new file using the default property lists.
            H5File file(FILE_NAME, H5F_ACC_TRUNC);

            // Create groups
            auto locations(file.createGroup("/locations"));
            auto location1(file.createGroup("/locations/location1"));

            // Create root attributes
            hsize_t dims1[1] = { 1 };
            DataSpace root_attr_ds = DataSpace (1, dims1);
            StrType root_attr_type(PredType::C_S1, 10);
            auto geogridName(file.createAttribute("geogridName", root_attr_type, root_attr_ds));
            geogridName.write(root_attr_type, "Vlaanderen");

            // Create location attributes
            hsize_t dims2[1] = { 1 };
            int attr_data[1] = { 2030 };
            int ptr_data = 100000000000;
            double lat = 1.12, lon = 5.43;
            DataSpace loc_attr_ds = DataSpace (1, dims2);
            StrType loc_name_type(PredType::C_S1, 9);
            StrType loc_province_type(PredType::C_S1, 9);
            auto loc_id(location1.createAttribute("id", PredType::NATIVE_INT, loc_attr_ds));
            auto name(location1.createAttribute("name", loc_name_type, loc_attr_ds));
            auto province(location1.createAttribute("province", loc_province_type, loc_attr_ds));
            auto population(location1.createAttribute("population", PredType::NATIVE_INT, loc_attr_ds));
            auto latitude(location1.createAttribute("latitude", PredType::NATIVE_DOUBLE, loc_attr_ds));
            auto longitude(location1.createAttribute("longitude", PredType::NATIVE_DOUBLE, loc_attr_ds));
            loc_id.write(PredType::NATIVE_INT, attr_data);
            name.write(loc_name_type, "Antwerpen");
            province.write(loc_province_type, "Antwerpen");
            population.write(PredType::NATIVE_INT, &ptr_data);
            latitude.write(PredType::NATIVE_DOUBLE, &lat);
            longitude.write(PredType::NATIVE_DOUBLE, &lon);


        }  // end of try block

            // catch failure caused by the H5File operations
        catch(FileIException &error)
        {
            Exception::printErrorStack();
        }

            // catch failure caused by the DataSet operations
        catch(DataSetIException &error)
        {
            Exception::printErrorStack();
        }

            // catch failure caused by the DataSpace operations
        catch(DataSpaceIException &error)
        {
            Exception::printErrorStack();
        }
    }

}