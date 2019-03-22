//
// Created by Andrei Bondarenko on 2019-03-20.
//

#include "util/FileSys.h"
#include <boost/property_tree/ptree.hpp>
#include <nlohmann/json.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iomanip>


using json = nlohmann::json;

namespace jsondemo {


    void run(){
        //
        // Create and write following json file
        //
        //{
        //  "pi": 3.141,
        //  "happy": true,
        //  "name": "Niels",
        //  "nothing": null,
        //  "answer": {
        //    "everything": 42
        //  },
        //  "list": [1, 0, 2],
        //  "object": {
        //    "currency": "USD",
        //    "value": 42.99
        //  }
        //}`
        //

        // create an empty structure (null)
        json j;

        // add a number that is stored as double (note the implicit conversion of j to an object)
                j["pi"] = 3.141;

        // add a Boolean that is stored as bool
                j["happy"] = true;

        // add a string that is stored as std::string
                j["name"] = "Niels";

        // add another null object by passing nullptr
                j["nothing"] = nullptr;

        // add an object inside the object
                j["answer"]["everything"] = 42;

        // add an array that is stored as std::vector (using an initializer list)
                j["list"] = { 1, 0, 2 };

        // add another object (using an initializer list of pairs)
                j["object"] = { {"currency", "USD"}, {"value", 42.99} };

        boost::filesystem::ofstream file("demo.json");
        file << std::setw(4) << j << std::endl;
        file.close();
    }

}