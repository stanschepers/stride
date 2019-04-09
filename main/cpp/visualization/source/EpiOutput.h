//
// Created by laurens on 4/10/19.
//

#ifndef STRIDE_EPIOUTPUT_H
#define STRIDE_EPIOUTPUT_H

#include <iostream>
#include <map>

namespace visualization {

//            location              ageBracket           healthStatus             day            value
typedef std::map<std::string, std::map<std::string, std::map<std::string, std::map<unsigned int, double>>>> EpiOutput;

}

#endif //STRIDE_EPIOUTPUT_H
