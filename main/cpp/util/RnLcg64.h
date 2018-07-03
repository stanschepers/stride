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
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Interface of RnLcg64.
 */

#include <functional>
#include <trng/lcg64.hpp>
#include <randutils/randutils.hpp>
#include <string>
#include <vector>

namespace stride {
namespace util {

/**
 * Manages random number generation in parallel (OpenMP) calculations.
 */
class RnLcg64 : protected std::vector<randutils::random_generator<trng::lcg64, randutils::seed_seq_fe128>>
{
public:
        using EngineType    = trng::lcg64;
        using GeneratorType = randutils::random_generator<trng::lcg64, randutils::seed_seq_fe128>;
        using ContainerType = std::vector<randutils::random_generator<trng::lcg64, randutils::seed_seq_fe128>>;

public:
        /// POD representation of the RNManager's state. If no state is available, i.e. state
        /// is an empty string, the initial state corresponding to the seed sequence is implied.
        /// If a state is available, the seed sequence is disregarded.
        struct Info
        {
                explicit Info(std::string seed_seq_init = "1,2,3,4", std::string state = "",
                              unsigned int stream_count = 1U)
                    : m_seed_seq_init(std::move(seed_seq_init)), m_state(std::move(state)),
                      m_stream_count(stream_count){};

                std::string  m_seed_seq_init; ///< Seed for the engine.
                std::string  m_state;         ///< Long string representing current state.
                unsigned int m_stream_count;  ///< Number of streams set up with the engine.
        };

public:
        using ContainerType::operator[];
        using ContainerType::at;
        using ContainerType::size;

        /// Initializes.
        explicit RnLcg64(const Info& info = Info());

        /// No copying.
        RnLcg64(const RnLcg64&) = delete;

        /// No copy assignment.
        RnLcg64& operator=(const RnLcg64&) = delete;

        /// Equality of states
        bool operator==(const RnLcg64& other);

        /// Return the state of the random engines.
        Info GetInfo() const;

private:
        /// Initalize with data in Info.
        void Seed(const Info& info = Info());

private:
        std::string  m_seed_seq_init; ///< Seed sequence initializer used with engines.
        unsigned int m_stream_count;  ///< Number of threads/streams set up with the engine.
};

} // namespace util
} // namespace stride