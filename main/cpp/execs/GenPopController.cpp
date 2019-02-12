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
 *  Copyright 2017, 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Header for the command line controller.
 */

#include "GenPopController.h"

#include "gengeopop/GeoGrid.h"
#include "gengeopop/io/GeoGridWriter.h"
#include "gengeopop/io/GeoGridWriterFactory.h"

#include "pop/GeoPopBuilder.h"
#include "pop/Population.h"
#include "util/FileSys.h"

#ifdef BOOST_FOUND
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
namespace filesys = boost::filesystem;
#else
#include <filesystem>
namespace filesys = std::filesystem;
#endif

#include <boost/property_tree/ptree.hpp>
#include <spdlog/spdlog.h>
#include <fstream>
#include <memory>

using namespace std;
using namespace stride::util;
using namespace gengeopop;
using namespace boost::property_tree;

namespace stride {

GenPopController::GenPopController(const ptree& configPt) : ControlHelper("GeoPopController", configPt) {}

void GenPopController::Control()
{
        // -----------------------------------------------------------------------------------------
        // Prelims.
        // -----------------------------------------------------------------------------------------
        CheckEnv();
        CheckOutputPrefix();
        InstallLogger();
        LogStartup();

        // --------------------------------------------------------------
        // Set up the GenPopBuilder and build population with GeoGrid.
        // --------------------------------------------------------------
        m_stride_logger->info("GenPopBuilder invoked.");
        GeoPopBuilder geoPopBuilder(m_config_pt, m_rn_manager, m_stride_logger);
        const auto    pop = Population::Create();

        geoPopBuilder.Build(pop);
        m_stride_logger->info("GenPopBuilder done.");

        // --------------------------------------------------------------
        // Write to file.
        // --------------------------------------------------------------
        const auto prefix      = m_config_pt.get<string>("run.output_prefix");
        const auto popFileName = m_config_pt.get<string>("run.population_file", "gengeopop.proto");
        const auto popFilePath = FileSys::BuildPath(prefix, popFileName);
        m_stride_logger->info("Writing to population file {}.", popFilePath.string());

        GeoGridWriterFactory      geoGridWriterFactory;
        shared_ptr<GeoGridWriter> geoGridWriter = geoGridWriterFactory.CreateWriter(popFileName);
        ofstream                  outputFileStream(popFilePath.string());

        geoGridWriter->Write(pop->GetGeoGrid(), outputFileStream);
        outputFileStream.close();
        m_stride_logger->info("Done writing to population to file {}.", popFileName);


        // -----------------------------------------------------------------------------------------
        // Shutdown.
        // -----------------------------------------------------------------------------------------
        LogShutdown();
        spdlog::drop_all();
}

} // namespace stride