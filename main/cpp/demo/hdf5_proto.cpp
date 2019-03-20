//
// Created by Andrei Bondarenko on 2019-03-20.
//

#include <H5Cpp.h>
#include <iostream>
#include <string>

using namespace H5;

namespace hdf5demo {

    void run(){
        const H5std_string	FILE_NAME("h5tutr_dset.h5");
        const H5std_string	DATASET_NAME("dset");
        const int	 NX = 4;                     // dataset dimensions
        const int	 NY = 6;
        const int	 RANK = 2;

        try
        {
            // Turn off the auto-printing when failure occurs so that we can
            // handle the errors appropriately
            Exception::dontPrint();

            // Create a new file using the default property lists.
            H5File file(FILE_NAME, H5F_ACC_TRUNC);

            // Create the data space for the dataset.
            hsize_t dims[2];               // dataset dimensions
            dims[0] = NX;
            dims[1] = NY;
            DataSpace dataspace(RANK, dims);

            // Create the dataset.
            DataSet dataset = file.createDataSet(DATASET_NAME, PredType::STD_I32BE, dataspace);

        }  // end of try block

            // catch failure caused by the H5File operations
        catch(FileIException error)
        {
            error.printErrorStack();
        }

            // catch failure caused by the DataSet operations
        catch(DataSetIException error)
        {
            error.printErrorStack();
        }

            // catch failure caused by the DataSpace operations
        catch(DataSpaceIException error)
        {
            error.printErrorStack();
        }
    }

}