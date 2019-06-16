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
 *  Copyright 2019, Andrei Bondarenko.
 */

#include <memory>

#include "EpiOutputHDF5Reader.h"
#include "geopop/io/H5Utils.h"

#include "contact/AgeBrackets.h"
#include "disease/Health.h"
#include "geopop/Location.h"

namespace visualization {

using namespace std;
using namespace stride;
using namespace H5;
using namespace H5Utils;
using namespace geopop;

EpiOutputHDF5Reader::EpiOutputHDF5Reader(const string& filename, GeoGrid<EpiOutput>* epiOutput)
    : EpiOutputReader(epiOutput)
{
        if (!filename.empty()) {
                m_input = H5File(filename, H5F_ACC_RDONLY);
        }
}

void EpiOutputHDF5Reader::Read()
{
        auto& geoGrid = *m_epiOutput;

        unsigned int day_count;
        ReadAttribute(day_count, "day_count", m_input);

        if (day_count > 0) {
                for (size_t idx = 0; idx < day_count; ++idx) {
                        Group day(m_input.openGroup("DAY" + to_string(idx)));
                        ReadDay(day, geoGrid);
                }
        }

        m_input.close();
}
void EpiOutputHDF5Reader::ReadDay(const Group& day, GeoGrid<EpiOutput>& grid)
{
        unsigned int day_num;
        unsigned long location_count;

        ReadAttribute(day_num, "day_num", day);
        ReadAttribute(location_count, "location_count", day);

        if (location_count > 0) {
                for (size_t idx = 0; idx < location_count; ++idx) {
                        DataSet location(day.openDataSet("LOC" + to_string(idx)));

                        unsigned int id, pop_count, province;
                        double       latitude, longitude;
                        string       name;
                        size_t       size;

                        ReadAttribute(id, "id", location);
                        ReadAttribute(pop_count, "population", location);
                        ReadAttribute(province, "province", location);
                        ReadAttribute(latitude, "latitude", location);
                        ReadAttribute(longitude, "longitude", location);
                        ReadAttribute(name, "name", location);
                        ReadAttribute(size, "size", location);

                        if (day_num == 0) {
                                auto locationPtr =
                                    make_shared<Location<EpiOutput>>(id, province, make_shared<EpiOutput>(),
                                                                     Coordinate(longitude, latitude), name);
                                grid.AddLocation(locationPtr);
                        }

                        grid[idx]->GetContent()->pop_count = pop_count;

                        vector<H5EpiOutput> epi_data(size);
                        location.read(epi_data.data(), GetCompoundType<H5EpiOutput>());

                        if (size > 0) {
                                auto& epiOutput = grid[idx]->GetContent()->epiOutput;
                                for (auto health_status : epi_data) {
                                        auto bracket    = ageBrackets[health_status.age_bracket];
                                        auto status     = healthStatuses[health_status.health_status];
                                        auto percentage = health_status.percentage;
                                        epiOutput[bracket][status][day_num] = percentage;
                                }
                        }
                }
        }
}

} // namespace visualization
