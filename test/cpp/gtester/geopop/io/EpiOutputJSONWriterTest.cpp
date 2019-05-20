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
 *  Copyright 2019, Laurens Van Damme
 */

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

#include "geopop/GeoGrid.h"
#include "geopop/io/EpiOutputJSONWriter.h"

using namespace std;
using namespace stride;
using namespace geopop;

namespace {

bool compareOutput(string expected, string actual)
{
        // Remove all whitespace to produce a minified JSON string
        expected.erase(remove_if(expected.begin(), expected.end(), ::isspace), expected.end());
        actual.erase(remove_if(actual.begin(), actual.end(), ::isspace), actual.end());

        return expected == actual;
}

/// Checks if the epi-output json writer creates the expected result with no locations in geogrid
TEST(EpiOutputJSONWriterTest, zeroLocationsTest) {
        string expectedOutput = R"({
            "measured_days": [
                        0
            ]
        })";

        auto geoGrid = GeoGrid<Epidemiologic>(nullptr);

        auto writer = EpiOutputJSONWriter();

        writer.Update(geoGrid, 0);

        stringstream      ss;
        writer.Write(ss);

        EXPECT_TRUE(compareOutput(expectedOutput, ss.str()));
}

/// Checks if the epi-output json writer creates the expected result with no time steps
TEST(EpiOutputJSONWriterTest, oneLocationZeroDaysTest) {
        string expectedOutput = R"({
            "measured_days": null
        })";

        auto writer = EpiOutputJSONWriter();

        stringstream      ss;
        writer.Write(ss);

        EXPECT_TRUE(compareOutput(expectedOutput, ss.str())); }

/// Checks if the epi-output json writer creates the expected result with a geogrid consisting of one location and over 1 time step
TEST(EpiOutputJSONWriterTest, oneLocationOneDayTest) {
        string expectedOutput = R"({
            "locations": [
                {
                    "coordinate": [
                        0.0,
                        0.0
                    ],
                    "epi-output": {
                        "College": {
                            "Immune": {
                                "0": 0.0
                            },
                            "Infected": {
                                "0": 0.1
                            },
                            "Infectious": {
                                "0": 0.0
                            },
                            "Recovered": {
                                "0": 0.0
                            },
                            "Susceptible": {
                                "0": 0.0
                            },
                            "Symptomatic": {
                                "0": 0.0
                            },
                            "Total": {
                                "0": 0.1
                            }
                        },
                        "Daycare": {
                            "Immune": {
                                "0": 0.0
                            },
                            "Infected": {
                                "0": 0.0
                            },
                            "Infectious": {
                                "0": 0.0
                            },
                            "Recovered": {
                                "0": 0.1
                            },
                            "Susceptible": {
                                "0": 0.1
                            },
                            "Symptomatic": {
                                "0": 0.0
                            },
                            "Total": {
                                "0": 0.2
                            }
                        },
                        "K12School": {
                            "Immune": {
                                "0": 0.1
                            },
                            "Infected": {
                                "0": 0.0
                            },
                            "Infectious": {
                                "0": 0.0
                            },
                            "Recovered": {
                                "0": 0.1
                            },
                            "Susceptible": {
                                "0": 0.0
                            },
                            "Symptomatic": {
                                "0": 0.0
                            },
                            "Total": {
                                "0": 0.2
                            }
                        },
                        "PreSchool": {
                            "Immune": {
                                "0": 0.0
                            },
                            "Infected": {
                                "0": 0.0
                            },
                            "Infectious": {
                                "0": 0.0
                            },
                            "Recovered": {
                                "0": 0.0
                            },
                            "Susceptible": {
                                "0": 0.1
                            },
                            "Symptomatic": {
                                "0": 0.0
                            },
                            "Total": {
                                "0": 0.1
                            }
                        },
                        "Senior": {
                            "Immune": {
                                "0": 0.02
                            },
                            "Infected": {
                                "0": 0.0
                            },
                            "Infectious": {
                                "0": 0.0
                            },
                            "Recovered": {
                                "0": 0.0
                            },
                            "Susceptible": {
                                "0": 0.1
                            },
                            "Symptomatic": {
                                "0": 0.0
                            },
                            "Total": {
                                "0": 0.12
                            }
                        },
                        "Workplace": {
                            "Immune": {
                                "0": 0.08
                            },
                            "Infected": {
                                "0": 0.1
                            },
                            "Infectious": {
                                "0": 0.0
                            },
                            "Recovered": {
                                "0": 0.1
                            },
                            "Susceptible": {
                                "0": 0.0
                            },
                            "Symptomatic": {
                                "0": 0.0
                            },
                            "Total": {
                                "0": 0.27999999999999997
                            }
                        }
                    },
                    "id": 1,
                    "name": "test",
                    "pop_count": 0,
                    "province": 1
                }
            ],
            "measured_days": [
                0
            ]
        })";

        // Create a location with contact pools
        ContactPool          pool(1, ContactType::Id::Household);
        ContactPool          pool2(1, ContactType::Id::Household);
        vector<unsigned int> ages = {45, 17, 5, 1, 75, 35, 25, 22, 2, 12};
        vector<Person>       persons{};
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons.emplace_back(Person());
        }
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons[i].SetAge(ages[i]);
                if (i > 1) {
                        persons[i].GetHealth().SetSusceptible();
                        if (i > 4) {
                                persons[i].GetHealth().StartInfection();
                        }
                        if (i > 6) {
                                persons[i].GetHealth().StopInfection();
                                if (i == 7) {
                                        persons[i].SetPoolId(ContactType::Id::Workplace, 1);
                                }
                        }
                } else {
                        persons[i].GetHealth().SetImmune();
                }
                pool.AddMember(&persons[i]);
                if (i != 0) {
                        pool2.AddMember(&persons[i]);
                }
        }
        Person p;
        p.SetAge(80);
        p.GetHealth().SetImmune();
        pool2.AddMember(&p);

        Location<Epidemiologic> test(1, 1, Coordinate(0.0, 0.0), "test", 0U);
        for (unsigned int i = 0; i < 4; i++) {
                test.GetContent()->RegisterPool(&pool, stride::ContactType::Id::Household);
        }
        test.GetContent()->RegisterPool(&pool2, stride::ContactType::Id::Household);

        auto geoGrid = GeoGrid<Epidemiologic>(nullptr);
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(test));

        auto writer = EpiOutputJSONWriter();

        writer.Update(geoGrid, 0);

        stringstream      ss;
        writer.Write(ss);

        EXPECT_TRUE(compareOutput(expectedOutput, ss.str()));
}

TEST(EpiOutputJSONWriterTest, twoLocationsFiveDaysTest) {
        string expectedOutput = R"({
                "locations": [
                        {
                            "coordinate": [
                                0.0,
                                0.0
                            ],
                            "epi-output": {
                                "College": {
                                    "Immune": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infected": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Susceptible": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    }
                                },
                                "Daycare": {
                                    "Immune": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    },
                                    "Susceptible": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.2,
                                        "10": 0.2,
                                        "20": 0.2,
                                        "30": 0.2,
                                        "40": 0.2,
                                        "50": 0.2
                                    }
                                },
                                "K12School": {
                                    "Immune": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    },
                                    "Susceptible": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.2,
                                        "10": 0.2,
                                        "20": 0.2,
                                        "30": 0.2,
                                        "40": 0.2,
                                        "50": 0.2
                                    }
                                },
                                "PreSchool": {
                                    "Immune": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Susceptible": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    }
                                },
                                "Senior": {
                                    "Immune": {
                                        "0": 0.02,
                                        "10": 0.02,
                                        "20": 0.02,
                                        "30": 0.02,
                                        "40": 0.02,
                                        "50": 0.02
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Susceptible": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.12,
                                        "10": 0.12,
                                        "20": 0.12,
                                        "30": 0.12,
                                        "40": 0.12,
                                        "50": 0.12
                                    }
                                },
                                "Workplace": {
                                    "Immune": {
                                        "0": 0.08,
                                        "10": 0.08,
                                        "20": 0.08,
                                        "30": 0.08,
                                        "40": 0.08,
                                        "50": 0.08
                                    },
                                    "Infected": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.1,
                                        "10": 0.1,
                                        "20": 0.1,
                                        "30": 0.1,
                                        "40": 0.1,
                                        "50": 0.1
                                    },
                                    "Susceptible": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.27999999999999997,
                                        "10": 0.27999999999999997,
                                        "20": 0.27999999999999997,
                                        "30": 0.27999999999999997,
                                        "40": 0.27999999999999997,
                                        "50": 0.27999999999999997
                                    }
                                }
                            },
                            "id": 1,
                            "name": "test0",
                            "pop_count": 0,
                            "province": 1
                        },
                        {
                            "coordinate": [
                                1.0,
                                1.0
                            ],
                            "epi-output": {
                                "College": {
                                    "Immune": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Susceptible": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    }
                                },
                                "Daycare": {
                                    "Immune": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Susceptible": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    }
                                },
                                "K12School": {
                                    "Immune": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Susceptible": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    }
                                },
                                "PreSchool": {
                                    "Immune": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Susceptible": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    }
                                },
                                "Senior": {
                                    "Immune": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Susceptible": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    }
                                },
                                "Workplace": {
                                    "Immune": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infected": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Infectious": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Recovered": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Susceptible": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Symptomatic": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    },
                                    "Total": {
                                        "0": 0.0,
                                        "10": 0.0,
                                        "20": 0.0,
                                        "30": 0.0,
                                        "40": 0.0,
                                        "50": 0.0
                                    }
                                }
                            },
                            "id": 1,
                            "name": "test1",
                            "pop_count": 100,
                            "province": 1
                        }
                    ],
                    "measured_days": [
                        0,
                        10,
                        20,
                        30,
                        40,
                        50
                    ]
        })";

        // Create a location with contact pools
        ContactPool          pool(1, ContactType::Id::Household);
        ContactPool          pool2(1, ContactType::Id::Household);
        vector<unsigned int> ages = {45, 17, 5, 1, 75, 35, 25, 22, 2, 12};
        vector<Person>       persons{};
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons.emplace_back(Person());
        }
        for (unsigned int i = 0; i < ages.size(); i++) {
                persons[i].SetAge(ages[i]);
                if (i > 1) {
                        persons[i].GetHealth().SetSusceptible();
                        if (i > 4) {
                                persons[i].GetHealth().StartInfection();
                        }
                        if (i > 6) {
                                persons[i].GetHealth().StopInfection();
                                if (i == 7) {
                                        persons[i].SetPoolId(ContactType::Id::Workplace, 1);
                                }
                        }
                } else {
                        persons[i].GetHealth().SetImmune();
                }
                pool.AddMember(&persons[i]);
                if (i != 0) {
                        pool2.AddMember(&persons[i]);
                }
        }
        Person p;
        p.SetAge(80);
        p.GetHealth().SetImmune();
        pool2.AddMember(&p);

        Location<Epidemiologic> test(1, 1, Coordinate(0.0, 0.0), "test0", 0U);
        for (unsigned int i = 0; i < 4; i++) {
                test.GetContent()->RegisterPool(&pool, stride::ContactType::Id::Household);
        }
        test.GetContent()->RegisterPool(&pool2, stride::ContactType::Id::Household);

        auto geoGrid = GeoGrid<Epidemiologic>(nullptr);
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(test));
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(1, 1, Coordinate(1, 1), "test1", 100));

        auto writer = EpiOutputJSONWriter();

        for (unsigned int i = 0; i < 51; i += 10)
                writer.Update(geoGrid, i);

        stringstream      ss;
        writer.Write(ss);

        EXPECT_TRUE(compareOutput(expectedOutput, ss.str()));
}

} // namespace
