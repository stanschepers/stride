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
 *  Copyright 2019, Jan Broeckhove and Bistromatics group.
 */

#ifndef STRIDE_EPIOUTPUT_H
#define STRIDE_EPIOUTPUT_H

#include <iostream>
#include <map>
#include <vector>

namespace visualization {

typedef struct
{
        std::string  name;
        unsigned int id;
        unsigned int pop_count;
        double       latitude;
        double       longitude;
        //        ageBracket           healthStatus             day        value
        std::map<std::string, std::map<std::string, std::map<unsigned int, double>>> epiOutput;
} Location;

//            location              ageBracket           healthStatus             day            value
// typedef std::map<std::string, std::map<std::string, std::map<std::string, std::map<unsigned int, double>>>>
// EpiOutput;
typedef std::vector<Location> EpiOutput;

} // namespace visualization

#endif // STRIDE_EPIOUTPUT_H