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
 *  Copyright 2019, Laurens Van Damme.
 */

#pragma once

#include <iostream>
#include <map>
#include <vector>

/**
 * Epi-output for use within the visualization, contains epi-output of location.
 */

struct EpiOutput{
    unsigned int pop_count = 0;
    //        ageBracket           healthStatus             day        value
    std::map<std::string, std::map<std::string, std::map<unsigned int, double>>> epiOutput{};
};



//for (const string& ageBracket : stride::ageBrackets) {
//        for (const string& healthStatus : stride::healthStatuses) {
//                std::cout << ageBracket << " - " << healthStatus << " = " << result[ageBracket][healthStatus] << std::endl;
//        }
//}
