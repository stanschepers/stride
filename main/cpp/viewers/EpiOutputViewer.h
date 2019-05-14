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

/**
 * @file
 * Observer for the epidemiological condition of the population each timestep.
 */

#pragma once

#include "sim/event/Id.h"
#include "geopop/io/EpiOutputWriter.h"

#include <memory>
#include <vector>
#include <utility>

namespace stride {

class SimRunner;

namespace viewers {

/// Viewer gathers infection count at each sim step.
class EpiOutputViewer
{
public:
    /// Instantiate cases viewer.
    explicit EpiOutputViewer(std::shared_ptr<SimRunner> runner, unsigned int stride,
                             std::ostream& stream, std::shared_ptr<geopop::EpiOutputWriter>& epi_output_writer) :
                             m_runner(std::move(runner)), m_stride(stride), m_current_step(0), m_stream(stream),
                             m_epi_output_writer(epi_output_writer) {}

    /// Let viewer perform update.
    void Update(sim_event::Id id);

private:
    std::shared_ptr<SimRunner> m_runner;
    unsigned int m_stride;
    unsigned int m_current_step;
    std::ostream& m_stream;
    std::shared_ptr<geopop::EpiOutputWriter> m_epi_output_writer;
};

} // namespace viewers
} // namespace stride
