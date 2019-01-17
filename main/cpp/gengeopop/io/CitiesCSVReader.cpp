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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#include "CitiesCSVReader.h"

#include "util/CSV.h"
#include "util/CSVRow.h"

namespace gengeopop {

using namespace std;

CitiesCSVReader::CitiesCSVReader(unique_ptr<istream> inputStream)
    : CitiesReader(move(inputStream)), m_reader(*(m_inputStream.get()))
{
}

void CitiesCSVReader::FillGeoGrid(shared_ptr<GeoGrid> geoGrid) const
{
        // cols: id   province  population  x_coord  y_coord  latitude  longitude  name
        vector<pair<shared_ptr<Location>, int>> addedLocations;

        unsigned int totalPopulation = 0;
        for (const stride::util::CSVRow& row : m_reader) {
                auto id  = row.GetValue<int>(0);
                auto loc = make_shared<Location>(id,                   // id
                                                 row.GetValue<int>(1), // province
                                                 // ignore x and y, not sure what they actually do
                                                 // then longitude & latitude
                                                 Coordinate(row.GetValue<double>(6), row.GetValue<double>(5)),
                                                 row.GetValue(7));

                geoGrid->AddLocation(loc);
                addedLocations.emplace_back(loc, row.GetValue<int>(2));
                totalPopulation += row.GetValue<int>(2);
        }

        for (const auto& loc : addedLocations) {
                loc.first->SetRelativePopulation(static_cast<double>(loc.second) /
                                                 static_cast<double>(totalPopulation));
        }
}

} // namespace gengeopop