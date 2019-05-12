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
 *  Copyright 2019, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Observer for SimEvents for commandline interface usage.
 */

#include "EpiOutputViewer.h"

#include "pop/Population.h"
#include "sim/Sim.h"
#include "sim/SimRunner.h"
#include "geopop/GeoGrid.h"

using namespace std;
using namespace stride::sim_event;

namespace stride {
namespace viewers {

void EpiOutputViewer::Update(const sim_event::Id id)
{
    switch (id) {
        case Id::AtStart:
        case Id::Stepped: {
            m_runner->GetSim()->GetPopulation()->RefGeoGrid(); //.doSomething();
            std::cout << m_stride << " Do something..." << std::endl; // TODO: implement update for epi-output viewer
            break;
        }
        default: break;
    }
}

} // namespace viewers
} // namespace stride
