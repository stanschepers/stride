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
 *  Copyright 2019, Jan Broeckhove.
 */

#include "WorkplaceCSVReader.h"

#include "util/CSV.h"

namespace geopop {

using namespace std;
using namespace stride::util;

WorkplaceCSVReader::WorkplaceCSVReader(std::unique_ptr<std::istream> inputStream)
    : m_input_stream(std::move(inputStream))
{
}

void WorkplaceCSVReader::SetWorkDistribution(std::vector<std::tuple<double, unsigned int, unsigned int>>& work_distribution)
{
        CSV reader(*(m_input_stream.get()));

        for (const CSVRow& row : reader) {

                std::tuple<double, unsigned int, unsigned int> temp;
                double ratio;
                unsigned int min_size;
                unsigned int max_size;
                try {
                        ratio = row.GetValue<double>(0);
                        min_size = row.GetValue<unsigned int>(1);
                        max_size = row.GetValue<unsigned int>(2);
                } catch (const std::bad_cast& e) {
                        // Incorrect type
                        throw e; //
                }
                temp = {ratio, min_size, max_size};
                work_distribution.emplace_back(temp);
        }
}

} // namespace geopop
