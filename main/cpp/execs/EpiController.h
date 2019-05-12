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
 * Header for the Epi(demiological output)Controller.
 */

#pragma once

#include "ControlHelper.h"

#include <boost/property_tree/ptree_fwd.hpp>

namespace stride {

    class EpiController : protected ControlHelper {
    public:
        /// Straight initialization.
        explicit EpiController(const boost::property_tree::ptree& config, const std::string& name = "EpiController");

        /// Actual run of the simulator.
        void Control();
    };

} // namespace stride
