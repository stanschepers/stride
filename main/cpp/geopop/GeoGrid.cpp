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
 *  Copyright 2018, 2019, Jan Broeckhove, Laurens Van Damme and Bistromatics group.
 */

#include "GeoGrid.h"

#include "contact/ContactPool.h"
#include "geopop/Location.h"
#include "geopop/geo/GeoAggregator.h"
#include "geopop/geo/GeoGridKdTree.h"
#include "pop/Population.h"

#include <queue>
#include <stdexcept>
#include <utility>

namespace geopop {

using namespace std;
using stride::ContactPool;
using stride::ContactType::Id;

template <class LocationContent>
GeoGrid<LocationContent>::GeoGrid(stride::Population* population)
    : m_locations(), m_id_to_index(), m_population(population), m_finalized(false), m_tree()
{
}

template <class LocationContent>
void GeoGrid<LocationContent>::AddLocation(shared_ptr<Location<LocationContent>> location)
{
        if (m_finalized) {
                throw std::runtime_error("Calling addLocation while GeoGrid is finalized not supported!");
        }
        m_locations.emplace_back(location);
        m_id_to_index[location->GetID()] = static_cast<unsigned int>(m_locations.size() - 1);
}

template <class LocationContent>
template <typename Policy, typename F>
GeoAggregator<Policy, F> GeoGrid<LocationContent>::BuildAggregator(F functor, typename Policy::Args&& args) const
{
        return GeoAggregator<Policy, F>(m_tree, functor, std::forward<typename Policy::Args>(args));
}

template <class LocationContent>
template <typename Policy>
GeoAggregator<Policy> GeoGrid<LocationContent>::BuildAggregator(typename Policy::Args&& args) const
{
        return GeoAggregator<Policy>(m_tree, std::forward<typename Policy::Args>(args));
}

template <class LocationContent>
void GeoGrid<LocationContent>::CheckFinalized(const string& functionName) const
{
        if (!m_finalized) {
                throw std::runtime_error("Calling \"" + functionName + "\" with GeoGrid not finalized not supported!");
        }
}

template <class LocationContent>
void GeoGrid<LocationContent>::Finalize()
{
        vector<geogrid_detail::KdTree2DPoint> points;
        for (const auto& loc : m_locations) {
                points.emplace_back(geogrid_detail::KdTree2DPoint(loc.get()));
        }
        m_tree      = GeoGridKdTree::Build(points);
        m_finalized = true;
}

template <class LocationContent>
set<const Location<LocationContent>*> GeoGrid<LocationContent>::LocationsInBox(double long1, double lat1, double long2, double lat2) const
{
        CheckFinalized(__func__);

        set<const Location<void>*> result;

        auto agg = BuildAggregator<BoxPolicy>(
            MakeCollector(inserter(result, result.begin())),
            make_tuple(min(long1, long2), min(lat1, lat2), max(long1, long2), max(lat1, lat2)));
        agg();

        set<const Location<LocationContent>*> final_result;
        for (const Location<void>* loc: result){
                final_result.emplace_back(reinterpret_cast<Location<LocationContent>*>(const_cast<Location<void>*>(loc)));
        }

        return final_result;
}

template <class LocationContent>
set<const Location<LocationContent>*> GeoGrid<LocationContent>::LocationsInBox(Location<LocationContent>* loc1, Location<LocationContent>* loc2) const
{
        using boost::geometry::get;
        return LocationsInBox(get<0>(loc1->GetCoordinate()), get<1>(loc1->GetCoordinate()),
                              get<0>(loc2->GetCoordinate()), get<1>(loc2->GetCoordinate()));
}

template <class LocationContent>
vector<const Location<LocationContent>*> GeoGrid<LocationContent>::LocationsInRadius(const Location<LocationContent>& start, double radius) const
{
        CheckFinalized(__func__);

        geogrid_detail::KdTree2DPoint startPt(reinterpret_cast<const Location<void>*>(const_cast<Location<LocationContent>*>(&start)));
        vector<const Location<void>*>       result;

        auto agg = BuildAggregator<RadiusPolicy>(MakeCollector(back_inserter(result)), make_tuple(startPt, radius));
        agg();

        vector<const Location<LocationContent>*> final_result;
        for (const Location<void>* loc: result){
            final_result.emplace_back(reinterpret_cast<const Location<LocationContent>*>(const_cast<Location<void>*>(loc)));
        }

        return final_result;
}

//template <>
//template <>
//vector<ContactPool*> GeoGrid<Epidemiologic>::GetNearbyPools(Id id, const Location<Epidemiologic>& start, double startRadius) const
//{
//        double               currentRadius = startRadius;
//        vector<ContactPool*> pools;
//
//        while (pools.empty()) {
//                for (const Location<Epidemiologic>* nearLoc : LocationsInRadius(start, currentRadius)) {
//                        const auto& locPool = nearLoc->GetContent()->CRefPools(id);
//                        pools.insert(pools.end(), locPool.begin(), locPool.end());
//                }
//                currentRadius *= 2;
//                if (currentRadius == numeric_limits<double>::infinity()) {
//                        break;
//                }
//        }
//        return pools;
//}

template <class LocationContent>
vector<Location<LocationContent>*> GeoGrid<LocationContent>::TopK(size_t k) const
{
        auto cmp = [](Location<LocationContent>* rhs, Location<LocationContent>* lhs) { return rhs->GetContent()->GetPopCount() >
                lhs->GetContent()->GetPopCount(); };

        priority_queue<Location<LocationContent>*, vector<Location<LocationContent>*>, decltype(cmp)> queue(cmp);
        for (const auto& loc : m_locations) {
                queue.push(loc.get());
                if (queue.size() > k) {
                        queue.pop();
                }
        }

        vector<Location<LocationContent>*> topLocations;
        while (!queue.empty()) {
                auto loc = queue.top();
                topLocations.push_back(loc);
                queue.pop();
        }

        return topLocations;
}

} // namespace geopop
