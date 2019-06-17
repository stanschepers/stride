#pragma once
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
 *  Copyright 2018, 2019, Jan Broeckhove and Bistromatics group.
 */

#include "contact/ContactPool.h"
#include "contact/IdSubscriptArray.h"
#include "pop/Person.h"
#include "util/SegmentedVector.h"

#include <boost/property_tree/ptree_fwd.hpp>

#include <memory>
#include <ostream>
#include <map>

namespace geopop {

template <class LocationContent>
class GeoGrid;


// -----------------------------------------------------------------------------------------
// The reference Households used to generate the population by random draws.
// -----------------------------------------------------------------------------------------
struct ReferenceHouseHold
{
        /// Number of persons in the reference household set.
        unsigned int person_count = 0U;

        /// Age profile per reference household.
        std::vector<std::vector<unsigned int>> ages{};

        /// Young/Old fraction of reference household
        double young_old_fraction = 1.0;
};

// -----------------------------------------------------------------------------------------
// These are numbers derived from the reference households, the target size of the generated
// population and the input parameters relating participation school and worplace.
// These numbers are used as targets in the poggen process and are reproduced (to very close
// approximation) in the generated population.
// -----------------------------------------------------------------------------------------
struct PopulationInfo
{
        /// Number of individuals in Daycare.
        unsigned int popcount_daycare;

        /// Number of individuals in PreSchool.
        unsigned int popcount_preschool;

        /// Number of individuals in K12School.
        unsigned int popcount_k12school;

        /// Number of individuals in College.
        unsigned int popcount_college;

        /// Number of individuals in Workplace.
        unsigned int popcount_workplace;

        /// Number of households.
        unsigned int count_households;
};

/**
 * Configuration data mostly for generating a population, but also for computing
 * the required number of schools, workplaces, communities etc. for that population.
 */
class GeoGridConfig
{
public:
        /// Default constructor needed in test code.
        GeoGridConfig();

        /// Constructor that configures input data.
        explicit GeoGridConfig(const boost::property_tree::ptree& configPt);

        /// People per unit (= Household, K12School, College, etc.) for each of the ContactTypes.
        /// Default initialization. Order in which contacttypes are listed in the
        /// definition of the enumeration must be respected!
        stride::ContactType::IdSubscriptArray<unsigned int> people {0U, 9U, 200U, 500U, 3000U, 20U, 2000U, 2000U};

        /// Pools per unit (= Household, K12School, College, etc.) for each of the ContactTypes.
        /// Default initialization. Order in which contacttypes are listed in the
        /// definition of the enumeration must be respected!
        stride::ContactType::IdSubscriptArray<unsigned int> pools {1U, 1U, 10U, 25U, 20U, 1U, 1U, 1U};

        // -----------------------------------------------------------------------------------------
        // Parameters set by constructor with configuration property tree.
        // -----------------------------------------------------------------------------------------
        struct
        {
                /// Participation of daycare (fraction of people of daycare age going to daycare).
                double participation_daycare;

                /// Participation of preschool (fraction of people of preschool age going to preschool).
                double participation_preschool;

                /// Participation of college (fraction of people of college age going to college).
                double participation_college;

                /// Participation of workplace (fraction of people of work age and not going to
                /// college and having employment).
                double participation_workplace;

                /// Fraction of college students that commute.
                double fraction_college_commuters;

                /// Fraction of people in the workplace that commute.
                double fraction_workplace_commuters;

                /// Target population size for the generated population.
                unsigned int pop_size;

                /// Distribution of pool-sizes for workplaces
                std::vector<std::tuple<double, unsigned int, unsigned int>> work_distribution;
        } param;

        ReferenceHouseHold refHH;

        PopulationInfo info;

        std::map<unsigned int, ReferenceHouseHold> refHHperHHType;

        std::map<unsigned int, PopulationInfo> popInfoperHHtype;

        // -----------------------------------------------------------------------------------------
        /// Read the househould data file, parse it and set data.
        // -----------------------------------------------------------------------------------------
        void SetData(const std::string& householdsFileName);

        void SetData(const std::map<unsigned int, std::string> &householdsFileNamePerId);


        // -----------------------------------------------------------------------------------------
        /// Read the workplace distribution data file, parse it and set data.
        // -----------------------------------------------------------------------------------------
        void SetDistributionData(const std::string& distributionFileName);

};

} // namespace geopop
