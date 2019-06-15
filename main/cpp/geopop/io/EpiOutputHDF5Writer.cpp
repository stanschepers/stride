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

#include "EpiOutputHDF5Writer.h"
#include "H5Utils.h"

#include "contact/AgeBrackets.h"
#include "disease/Health.h"
#include "geopop/GeoGrid.h"
#include "geopop/Location.h"

#include <iostream>

namespace geopop {

class Epidemiologic;

using namespace std;
using namespace stride;
using namespace H5;
using namespace H5Utils;

EpiOutputHDF5Writer::EpiOutputHDF5Writer(string const& filename) : m_day_count(0), m_location_count(0)
{
        if (!filename.empty()) {
                m_output = H5File(filename, H5F_ACC_TRUNC);
        }
}

void EpiOutputHDF5Writer::Write()
{
        WriteAttribute(m_day_count, "day_count", m_output);
        m_output.close();
}

void EpiOutputHDF5Writer::Update(GeoGrid<Epidemiologic>& geoGrid, unsigned int day)
{
        Group day_group(m_output.createGroup("/DAY" + to_string(m_day_count++)));

        WriteAttribute(day, "day_num", day_group);
        WriteAttribute(geoGrid.size(), "location_count", day_group);

        CompType health_status_t = GetCompoundType<H5EpiOutput>();
        size_t   dataset_size    = ageBrackets.size() * healthStatuses.size();

        for (const auto& loc : geoGrid) {
                DataSet locData(day_group.createDataSet("LOC" + to_string(m_location_count++), health_status_t,
                                                        CreateSpace(dataset_size)));

                WriteAttribute(loc->GetID(), "id", locData);
                WriteAttribute(loc->GetName(), "name", locData);
                WriteAttribute(loc->GetProvince(), "province", locData);
                WriteAttribute(loc->GetContent()->GetPopCount(), "population", locData);
                WriteAttribute(loc->get<0>(), "longitude", locData);
                WriteAttribute(loc->get<1>(), "latitude", locData);
                WriteAttribute(dataset_size, "size", locData);

                auto                epiOutput = loc->GetContent()->GenerateEpiOutput();
                vector<H5EpiOutput> health_statuses(dataset_size);

                for (unsigned long i = 0; i < ageBrackets.size(); ++i) {
                        string age_bracket = ageBrackets[i];
                        for (unsigned int j = 0; j < healthStatuses.size(); ++j) {
                                string       health_status         = healthStatuses[j];
                                unsigned int idx                   = (i * healthStatuses.size()) + j;
                                health_statuses[idx].age_bracket   = i;
                                health_statuses[idx].health_status = j;
                                health_statuses[idx].percentage    = epiOutput[age_bracket][health_status];
                        }
                }
                locData.write(health_statuses.data(), health_status_t);
        }
        m_location_count = 0;
}

} // namespace geopop
