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
 *  Copyright 2018, Jan Broeckhove and Bistromatics group.
 */

#pragma once

#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"

/// Setup of the GeoGrid in the population.
template <typename Generator>
void SetupGeoGrid(int locCount, int locPop, int schoolCount, int houseHoldCount, int personCount,
                  stride::Population* pop, stride::ContactType::Id id);

/// Setup of the GeoGrid in the population with K12Schools.
void SetupK12SchoolGeoGrid(int locCount, int locPop, int k12SchoolCount, int houseHoldCount, int personCount,
                           stride::Population* pop);

/// Setup of the GeoGrid in the population with Daycares.
void SetupDaycareGeoGrid(int locCount, int locPop, int daycareCount, int houseHoldCount, int personCount,
                         stride::Population* pop);

/// Setup of the GeoGrid in the population with PreSchools.
void SetupPreSchoolGeoGrid(int locCount, int locPop, int preSchoolCount, int houseHoldCount, int personCount,
                           stride::Population* pop);