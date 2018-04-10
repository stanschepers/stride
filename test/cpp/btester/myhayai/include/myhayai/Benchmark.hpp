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
 *
 *  This software has been altered form the hayai software by Nick Bruun.
 *  The original copyright, to be found in the directory two levels higher
 *  still aplies.
 */
/**
 * @file
 * Header file for Benchmark.
 */

#include "Benchmarker.hpp"
#include "InfoFactory.hpp"
#include "TestFactory.hpp"

#include <utility>

namespace myhayai {

class Benchmark
{
public:
        ///
        Benchmark(const char* fixtureName, const char* testName, std::size_t numRuns, TestFactory testFactory)
            : Benchmark(fixtureName, testName, numRuns, std::move(testFactory), InfoFactory(), false){};

        ///
        Benchmark(const char* fixtureName, const char* testName, std::size_t numRuns, TestFactory testFactory,
                  bool disableTest)
            : Benchmark(fixtureName, testName, numRuns, std::move(testFactory), InfoFactory(), disableTest){};

        ///
        Benchmark(const char* fixtureName, const char* testName, std::size_t numRuns, TestFactory testFactory,
                  InfoFactory infoFactory, bool disableTest = false)
        {
                Benchmarker::RegisterTest(fixtureName, testName, numRuns, std::move(testFactory),
                                          std::move(infoFactory), disableTest);
        }
};

} // namespace myhayai
