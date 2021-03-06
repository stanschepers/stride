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
 *  Copyright 2018, 2019 Jan Broeckhove and Bistromatics group.
 */

/**
 * @file
 * Initialize populations: implementation. NOTICE: WorkplacePopulator logic
 * requires that CollegePopulator is executed prior to WorkplacePopulator.
 */

#include "GeoPopBuilder.h"

#include "geopop/GeoGrid.h"
#include "geopop/Epidemiologic.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/generators/Generator.h"
#include "geopop/io/ReaderFactory.h"
#include "geopop/populators/Populator.h"
#include "pop/Population.h"
#include "pop/SurveySeeder.h"
#include "util/FileSys.h"

#include "util/RnMan.h"
#include "util/StringUtils.h"

#include <boost/property_tree/ptree.hpp>
#include <contact/ContactType.h>
#include <spdlog/logger.h>

namespace stride {

using namespace std;
using namespace util;
using namespace ContactType;
using namespace boost::property_tree;
using namespace geopop;

GeoPopBuilder::GeoPopBuilder(const ptree& config, RnMan& rnMan, shared_ptr<spdlog::logger> strideLogger)
    : AbstractPopBuilder(config, rnMan, move(strideLogger))
{
}

shared_ptr<Population> GeoPopBuilder::Build(shared_ptr<Population> pop)
{
        m_stride_logger->trace("Building geopop.");

        // ------------------------------------------------------------
        // Set the GeoGridConfig.
        // ------------------------------------------------------------
        GeoGridConfig ggConfig(m_config);
        auto configGeoPop = m_config.get_child("run.geopop_gen");
        if(configGeoPop.find("household_file") == configGeoPop.not_found()){
                map<unsigned int, string> householdsPerId = {};
                for(const auto& hh:configGeoPop.get_child("households")){
                        unsigned int popId = hh.second.get<int>("population_id");
                        string hhFile = hh.second.get<string>("household_file");
                        householdsPerId.insert(make_pair(popId, hhFile));
                }
                ggConfig.SetData(householdsPerId);
        } else {
                ggConfig.SetData(m_config.get<string>("run.geopop_gen.household_file"));
        }

        try {
                ggConfig.SetDistributionData(m_config.get<string>("run.geopop_gen.distribution_file"));
                m_stride_logger->trace("Detected work distribution file.");
        } catch (ptree_bad_path& e){
                m_stride_logger->trace("No distribution file detected.");
        }


        // ------------------------------------------------------------
        // Get GeoGrid associated with 'pop'.
        // ------------------------------------------------------------
        auto& geoGrid = pop->RefGeoGrid();

        // ------------------------------------------------------------
        // Read locations file (commute file only if present).
        // ------------------------------------------------------------
        string commutesFile;
        auto   geopop_gen = m_config.get_child("run.geopop_gen");
        if (geopop_gen.count("commuting_file")) {
                commutesFile = m_config.get<string>("run.geopop_gen.commuting_file");
        }

        m_stride_logger->trace("Starting MakeLocations");
        MakeLocations(geoGrid, ggConfig, m_config.get<string>("run.geopop_gen.cities_file"), commutesFile);
        m_stride_logger->trace("Finished MakeLocations");

        // ------------------------------------------------------------
        // Generate Geo.
        // ------------------------------------------------------------
        m_stride_logger->trace("Starting MakePools");
        MakePools(geoGrid, ggConfig);
        m_stride_logger->trace("Finished MakePools");

        // ------------------------------------------------------------
        // Generate Pop.
        // ------------------------------------------------------------
        m_stride_logger->trace("Starting MakePersons");
        MakePersons(geoGrid, ggConfig);
        m_stride_logger->trace("Finished MakePersons");

        // ------------------------------------------------------------
        // Done.
        // ------------------------------------------------------------
        m_stride_logger->trace("Done building geopop.");

        return pop;
}

void GeoPopBuilder::MakeLocations(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& geoGridConfig, const string& citiesFileName,
                                  const string& commutingFileName)
{
        const auto locationsReader = ReaderFactory::CreateLocationsReader(citiesFileName);
        locationsReader->FillGeoGrid(geoGrid);

        if (!commutingFileName.empty()) {
                const auto commutesReader = ReaderFactory::CreateCommutesReader(commutingFileName);
                commutesReader->FillGeoGrid(geoGrid);
        }

        for (const shared_ptr<Location<Epidemiologic>>& loc : geoGrid) {
            loc->GetContent()->SetPopCount(geoGridConfig.param.pop_size);
        }
        geoGrid.Finalize();
}

void GeoPopBuilder::MakePools(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& geoGridConfig)
{
        DaycareGenerator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        PreSchoolGenerator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        K12SchoolGenerator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        CollegeGenerator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        WorkplaceGenerator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        PrimaryCommunityGenerator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        SecondaryCommunityGenerator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        HouseholdGenerator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);
}

void GeoPopBuilder::MakePersons(GeoGrid<Epidemiologic>& geoGrid, const GeoGridConfig& geoGridConfig)
{
        // NOTICE: WorkplacePopulator logic requires that CollegePopulator
        // has been executed prior to WorkplacePopulator.

        HouseholdPopulator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        DaycarePopulator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        PreSchoolPopulator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        K12SchoolPopulator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        CollegePopulator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        PrimaryCommunityPopulator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        SecondaryCommunityPopulator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);

        WorkplacePopulator(m_rn_man, m_stride_logger).Apply(geoGrid, geoGridConfig);
}

} // namespace stride
