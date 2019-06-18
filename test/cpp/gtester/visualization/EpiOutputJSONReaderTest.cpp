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

#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <iostream>

#include "geopop/GeoGrid.h"
#include "visualization/source/EpiOutputJSONReader.h"

using namespace std;
using namespace stride;
using namespace geopop;
using namespace visualization;

namespace {

void getEpiOutputFromJSON(const string& jsonString, geopop::GeoGrid<EpiOutput> *epiOutput)
{
        auto instream = make_unique<istringstream>(jsonString);

        EpiOutputJSONReader epiOutputJSONReader(move(instream), epiOutput);

        epiOutputJSONReader.Read();
}

/// Checks if the epi-output json reader creates the expected geoGrid with no time steps
TEST(EpiOutputJSONReaderTest, zeroLocationsTest) {
        string jsonString = R"({
            "measured_days": [
                        0
            ]
        })";

        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        getEpiOutputFromJSON(jsonString, &geoGrid);

        EXPECT_EQ(0, geoGrid.size());
}

/// Checks if the epi-output json reader creates the expected geoGrid consisting of one location with epi-output for 0 time steps
TEST(EpiOutputJSONReaderTest, oneLocationZeroDaysTest) {
        string jsonString = R"({
            "measured_days": [
            ]
        })";

        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        getEpiOutputFromJSON(jsonString, &geoGrid);

        EXPECT_EQ(0, geoGrid.size());
}

/// Checks if the epi-output json reader creates the expected geoGrid consisting of one location with epi-output for 1 time step
TEST(EpiOutputJSONReaderTest, oneLocationOneDayTest) {
        string jsonString = R"({
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

        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        getEpiOutputFromJSON(jsonString, &geoGrid);

        EXPECT_EQ(1, geoGrid.size());

        EXPECT_EQ(1, geoGrid[0]->GetID());
        EXPECT_EQ("test", geoGrid[0]->GetName());
        EXPECT_EQ(geopop::Coordinate(0,0).get<0>(), geoGrid[0]->GetCoordinate().get<0>());
        EXPECT_EQ(geopop::Coordinate(0,0).get<1>(), geoGrid[0]->GetCoordinate().get<1>());
        EXPECT_EQ(1, geoGrid[0]->GetProvince());
        EXPECT_EQ(0, geoGrid[0]->GetContent()->pop_count);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Susceptible"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Recovered"][0]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Susceptible"][0]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["K12School"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Recovered"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Immune"][0]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Infected"][0]);

        EXPECT_EQ(0.27999999999999997, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Infected"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Recovered"][0]);
        EXPECT_EQ(0.08, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Immune"][0]);

        EXPECT_EQ(0.12, geoGrid[0]->GetContent()->epiOutput["Senior"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Senior"]["Susceptible"][0]);
        EXPECT_EQ(0.02, geoGrid[0]->GetContent()->epiOutput["Senior"]["Immune"][0]);
}

/// Checks if the epi-output json reader creates the expected geoGrid consisting of two locations with epi-output for 5 time steps
TEST(EpiOutputJSONReaderTest, twoLocationsFiveDayTest) {
        string jsonString = R"({
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
                            "id": 0,
                            "name": "test0",
                            "pop_count": 0,
                            "province": 0
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

        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        getEpiOutputFromJSON(jsonString, &geoGrid);

        EXPECT_EQ(2, geoGrid.size());

        EXPECT_EQ(0, geoGrid[0]->GetID());
        EXPECT_EQ("test0", geoGrid[0]->GetName());
        EXPECT_EQ(geopop::Coordinate(0,0).get<0>(), geoGrid[0]->get<0>());
        EXPECT_EQ(geopop::Coordinate(0,0).get<1>(), geoGrid[0]->get<1>());
        EXPECT_EQ(0, geoGrid[0]->GetProvince());
        EXPECT_EQ(0, geoGrid[0]->GetContent()->pop_count);

        EXPECT_EQ(1, geoGrid[1]->GetID());
        EXPECT_EQ("test1", geoGrid[1]->GetName());
        EXPECT_EQ(geopop::Coordinate(1.0,1.0).get<0>(), geoGrid[1]->get<0>());
        EXPECT_EQ(geopop::Coordinate(1.0,1.0).get<1>(), geoGrid[1]->get<1>());
        EXPECT_EQ(1, geoGrid[1]->GetProvince());
        EXPECT_EQ(100, geoGrid[1]->GetContent()->pop_count);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Susceptible"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Recovered"][0]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Susceptible"][0]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["K12School"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Recovered"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Immune"][0]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Infected"][0]);

        EXPECT_EQ(0.27999999999999997, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Infected"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Recovered"][0]);
        EXPECT_EQ(0.08, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Immune"][0]);

        EXPECT_EQ(0.12, geoGrid[0]->GetContent()->epiOutput["Senior"]["Total"][0]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Senior"]["Susceptible"][0]);
        EXPECT_EQ(0.02, geoGrid[0]->GetContent()->epiOutput["Senior"]["Immune"][0]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Susceptible"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Recovered"][10]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Susceptible"][10]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["K12School"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Recovered"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Immune"][10]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Infected"][10]);

        EXPECT_EQ(0.27999999999999997, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Infected"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Recovered"][10]);
        EXPECT_EQ(0.08, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Immune"][10]);

        EXPECT_EQ(0.12, geoGrid[0]->GetContent()->epiOutput["Senior"]["Total"][10]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Senior"]["Susceptible"][10]);
        EXPECT_EQ(0.02, geoGrid[0]->GetContent()->epiOutput["Senior"]["Immune"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Daycare"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Daycare"]["Susceptible"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Daycare"]["Recovered"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["PreSchool"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["PreSchool"]["Susceptible"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["K12School"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["K12School"]["Recovered"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["K12School"]["Immune"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["College"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["College"]["Infected"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Workplace"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Workplace"]["Infected"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Workplace"]["Recovered"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Workplace"]["Immune"][10]);

        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Senior"]["Total"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Senior"]["Susceptible"][10]);
        EXPECT_EQ(0, geoGrid[1]->GetContent()->epiOutput["Senior"]["Immune"][10]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Susceptible"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Daycare"]["Recovered"][50]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["PreSchool"]["Susceptible"][50]);

        EXPECT_EQ(0.2, geoGrid[0]->GetContent()->epiOutput["K12School"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Recovered"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["K12School"]["Immune"][50]);

        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["College"]["Infected"][50]);

        EXPECT_EQ(0.27999999999999997, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Infected"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Recovered"][50]);
        EXPECT_EQ(0.08, geoGrid[0]->GetContent()->epiOutput["Workplace"]["Immune"][50]);

        EXPECT_EQ(0.12, geoGrid[0]->GetContent()->epiOutput["Senior"]["Total"][50]);
        EXPECT_EQ(0.1, geoGrid[0]->GetContent()->epiOutput["Senior"]["Susceptible"][50]);
        EXPECT_EQ(0.02, geoGrid[0]->GetContent()->epiOutput["Senior"]["Immune"][50]);
}

/// Checks if the epi-output json reader throws an exception when the json is empty
TEST(EpiOutputJSONReaderTest, emptyStreamTest) {
        string jsonString = R"()";
        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        EXPECT_THROW(getEpiOutputFromJSON(jsonString, &geoGrid), runtime_error);
}

/// Checks if the epi-output json reader throws an exception when the json is invalid
TEST(EpiOutputJSONReaderTest, invalidJSONTest) {
        string jsonString = R"(
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
        auto geoGrid = GeoGrid<EpiOutput>(nullptr);
        EXPECT_THROW(getEpiOutputFromJSON(jsonString, &geoGrid), runtime_error);
}

} // namespace