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
 * Header for the command line controller.
 */

#include "EpiController.h"

#include "pop/Population.h"
#include "sim/Sim.h"
#include "sim/SimRunner.h"
#include "viewers/EpiOutputViewer.h"

#include "geopop/io/EpiOutputWriter.h"
#include "geopop/io/EpiOutputWriterFactory.h"
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
//#include <ostream>
#include <fstream>

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace boost::property_tree;

namespace stride {

    EpiController::EpiController(const ptree &config, const string &name) : ControlHelper(config, name) {}

    void EpiController::Control()
    {
        // -----------------------------------------------------------------------------------------
        // Prelims.
        // -----------------------------------------------------------------------------------------
        CheckEnv();
        CheckOutputPrefix();
        InstallLogger();
        LogStartup();

        // -----------------------------------------------------------------------------------------
        // Epi scenario: step 1, build a random number manager.
        // -----------------------------------------------------------------------------------------
        const RnInfo info{m_config.get<string>("run.rng_seed", "1,2,3,4"), "",
                          m_config.get<unsigned int>("run.num_threads")};
        RnMan        rnMan{info};

        // -----------------------------------------------------------------------------------------
        // Epi scenario: step 2, create a population, as described by the parameter in the config.
        // -----------------------------------------------------------------------------------------
        m_stride_logger->info("Generating the population...");
        auto pop = Population::Create(m_config, rnMan);

        // -----------------------------------------------------------------------------------------
        // Epi scenario: step 3, create a simulator, as described by the parameter in the config.
        // -----------------------------------------------------------------------------------------
        auto sim = Sim::Create(m_config, pop, rnMan);

        // -----------------------------------------------------------------------------------------
        // Epi scenario: step , build a runner, register viewers and run.
        // -----------------------------------------------------------------------------------------
        auto runner = make_shared<SimRunner>(m_config, sim);
        const auto fileFormat = m_config.get<std::string>("run.file_format");

        const auto prefix      = m_config.get<string>("run.output_prefix");
        const auto epiOutputFileName = "epiOutput." + fileFormat;
        const auto epiOutputFilePath = FileSys::BuildPath(prefix, epiOutputFileName);

        shared_ptr<geopop::EpiOutputWriter> epiOutputWriter = geopop::EpiOutputWriterFactory::CreateEpiOutputWriter(epiOutputFileName);
        std::ofstream                  outputFileStream(epiOutputFilePath.string());

        const auto epiStride = m_config.get<unsigned int>("run.epi_stride");
        auto eoViewer = make_shared<viewers::EpiOutputViewer>(runner, epiStride, outputFileStream, epiOutputWriter);
        runner->Register(eoViewer, bind(&viewers::EpiOutputViewer::Update, eoViewer, std::placeholders::_1));

        m_stride_logger->info("Starting the simulation and writing the epi-output with a stride of {} to file {}.", epiStride, epiOutputFilePath.string());

        runner->Run();
        epiOutputWriter->Write(outputFileStream);
        outputFileStream.close();

        m_stride_logger->info("Simulation over and done writing epi-output to file.");
    }

} // namespace stride


