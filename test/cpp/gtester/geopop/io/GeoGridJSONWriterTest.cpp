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

#include "geopop/io/GeoGridJSONWriter.h"
#include "GeoGridIOUtils.h"
#include "geopop/GeoGridConfig.h"
#include "pop/Population.h"
#include "util/FileSys.h"

#include <algorithm>
#include <cctype>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

bool compareOutput(string expected, string actual)
{
        // Remove all whitespace to produce a minified JSON string
        expected.erase(remove_if(expected.begin(), expected.end(), ::isspace), expected.end());
        actual.erase(remove_if(actual.begin(), actual.end(), ::isspace), actual.end());

        return expected == actual;
}

TEST(GeoGridJSOWriterTest, writeLocationsTest)
{
        string expectedOutput = R"({
            "locations": [
                {
                    "commutes": [],
                    "contactPools": [
                        {
                            "class": "Household",
                            "pools": []
                        },
                        {
                            "class": "Daycare",
                            "pools": []
                        },
                        {
                            "class": "PreSchool",
                            "pools": []
                        },
                        {
                            "class": "K12School",
                            "pools": []
                        },
                        {
                            "class": "College",
                            "pools": []
                        },
                        {
                            "class": "Workplace",
                            "pools": []
                        },
                        {
                            "class": "PrimaryCommunity",
                            "pools": []
                        },
                        {
                            "class": "SecondaryCommunity",
                            "pools": []
                        }
                    ],
                    "coordinate": {
                        "latitude": 0.0,
                        "longitude": 0.0
                    },
                    "id": 1,
                    "name": "Bavikhove",
                    "population": 2500,
                    "province": 4
                },
                {
                    "commutes": [],
                    "contactPools": [
                        {
                            "class": "Household",
                            "pools": []
                        },
                        {
                            "class": "Daycare",
                            "pools": []
                        },
                        {
                            "class": "PreSchool",
                            "pools": []
                        },
                        {
                            "class": "K12School",
                            "pools": []
                        },
                        {
                            "class": "College",
                            "pools": []
                        },
                        {
                            "class": "Workplace",
                            "pools": []
                        },
                        {
                            "class": "PrimaryCommunity",
                            "pools": []
                        },
                        {
                            "class": "SecondaryCommunity",
                            "pools": []
                        }
                    ],
                    "coordinate": {
                        "latitude": 0.0,
                        "longitude": 0.0
                    },
                    "id": 2,
                    "name": "Gent",
                    "population": 5000,
                    "province": 3
                },
                {
                    "commutes": [],
                    "contactPools": [
                        {
                            "class": "Household",
                            "pools": []
                        },
                        {
                            "class": "Daycare",
                            "pools": []
                        },
                        {
                            "class": "PreSchool",
                            "pools": []
                        },
                        {
                            "class": "K12School",
                            "pools": []
                        },
                        {
                            "class": "College",
                            "pools": []
                        },
                        {
                            "class": "Workplace",
                            "pools": []
                        },
                        {
                            "class": "PrimaryCommunity",
                            "pools": []
                        },
                        {
                            "class": "SecondaryCommunity",
                            "pools": []
                        }
                    ],
                    "coordinate": {
                        "latitude": 0.0,
                        "longitude": 0.0
                    },
                    "id": 3,
                    "name": "Mons",
                    "population": 2500,
                    "province": 2
                }
            ],
            "persons": []
        })";

        auto pop     = Population::Create();
        auto geoGrid = GeoGrid<Epidemiologic>(pop.get());
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Bavikhove", 2500));
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(2, 3, Coordinate(0, 0), "Gent", 5000));
        geoGrid.AddLocation(make_shared<Location<Epidemiologic>>(3, 2, Coordinate(0, 0), "Mons", 2500));

        shared_ptr<stringstream> ss = make_shared<stringstream>();
        GeoGridJSONWriter        writer(ss);
        writer.Write(geoGrid);

        EXPECT_TRUE(compareOutput(expectedOutput, ss->str()));
}

TEST(GeoGridJSOWriterTest, writePeopleTest)
{
        string expectedOutput = R"({
            "locations": [
                {
                    "commutes": [],
                    "contactPools": [
                        {
                            "class": "Household",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": [
                                        0
                                    ]
                                }
                            ]
                        },
                         {
                             "class": "Daycare",
                             "pools": [
                                 {
                                     "id": 1,
                                     "people": [
                                         0
                                     ]
                                 }
                             ]
                         },
                         {
                             "class": "PreSchool",
                             "pools": [
                                 {
                                     "id": 1,
                                     "people": [
                                         0
                                     ]
                                 }
                             ]
                         },
                        {
                            "class": "K12School",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": [
                                        0
                                    ]
                                }
                            ]
                        },
                        {
                            "class": "College",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": [
                                        0
                                    ]
                                }
                            ]
                        },
                        {
                            "class": "Workplace",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": [
                                        0
                                    ]
                                }
                            ]
                        },
                        {
                            "class": "PrimaryCommunity",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": [
                                        0
                                    ]
                                }
                            ]
                        },
                        {
                            "class": "SecondaryCommunity",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": [
                                        0
                                    ]
                                }
                            ]
                        }
                    ],
                    "coordinate": {
                        "latitude": 0.0,
                        "longitude": 0.0
                    },
                    "id": 1,
                    "name": "Bavikhove",
                    "population": 2500,
                    "province": 4
                }
            ],
            "persons": [
                {
                    "College": 1,
                    "Household": 1,
                    "K12School": 1,
                    "PrimaryCommunity": 1,
                    "SecondaryCommunity": 1,
                    "Workplace": 1,
                    "age": 18,
                    "id": 0
                }
            ]
        })";

        auto pop        = Population::Create();
        auto geoGridPtr = GetPopulatedGeoGrid(pop.get());

        shared_ptr<stringstream> ss = make_shared<stringstream>();
        GeoGridJSONWriter        writer(ss);
        writer.Write(*geoGridPtr);

        EXPECT_TRUE(compareOutput(expectedOutput, ss->str()));
}

TEST(GeoGridJSOWriterTest, writeContactPoolsTest)
{
        string expectedOutput = R"({
            "locations": [
                {
                    "commutes": [],
                    "contactPools": [
                        {
                            "class": "Household",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": []
                                }
                            ]
                        },
                        {
                            "class": "Daycare",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": []
                                }
                            ]
                        },
                        {
                            "class": "PreSchool",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": []
                                }
                            ]
                        },
                        {
                            "class": "K12School",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": []
                                }
                            ]
                        },
                        {
                            "class": "College",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": []
                                }
                            ]
                        },
                        {
                            "class": "Workplace",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": []
                                }
                            ]
                        },
                        {
                            "class": "PrimaryCommunity",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": []
                                }
                            ]
                        },
                        {
                            "class": "SecondaryCommunity",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": []
                                }
                            ]
                        }
                    ],
                    "coordinate": {
                        "latitude": 0.0,
                        "longitude": 0.0
                    },
                    "id": 1,
                    "name": "Bavikhove",
                    "population": 2500,
                    "province": 4
                }
            ],
            "persons": []
        })";

        auto pop      = Population::Create();
        auto geoGrid  = GeoGrid<Epidemiologic>(pop.get());
        auto location = make_shared<Location<Epidemiologic>>(1, 4, Coordinate(0, 0), "Bavikhove", 2500);

        for (auto type : IdList) {
                auto contactPoolPtr = pop->RefPoolSys().CreateContactPool(type);
                location->GetContent()->RefPools(type).emplace_back(contactPoolPtr);
        }

        geoGrid.AddLocation(location);

        shared_ptr<stringstream> ss = make_shared<stringstream>();
        GeoGridJSONWriter        writer(ss);
        writer.Write(geoGrid);

        EXPECT_TRUE(compareOutput(expectedOutput, ss->str()));
}

TEST(GeoGridJSOWriterTest, writeCommutesTest)
{
        string expectedOutput = R"({
            "locations": [
                {
                    "commutes": [
                        {
                            "proportion": 0.5,
                            "to": 2
                        },
                        {
                            "proportion": 0.25,
                            "to": 3
                        }
                    ],
                    "contactPools": [
                        {
                            "class": "Household",
                            "pools": []
                        },
                        {
                            "class": "Daycare",
                            "pools": []
                        },
                        {
                            "class": "PreSchool",
                            "pools": []
                        },
                        {
                            "class": "K12School",
                            "pools": []
                        },
                        {
                            "class": "College",
                            "pools": []
                        },
                        {
                            "class": "Workplace",
                            "pools": []
                        },
                        {
                            "class": "PrimaryCommunity",
                            "pools": []
                        },
                        {
                            "class": "SecondaryCommunity",
                            "pools": []
                        }
                    ],
                    "coordinate": {
                        "latitude": 0.0,
                        "longitude": 0.0
                    },
                    "id": 1,
                    "name": "Bavikhove",
                    "population": 2500,
                    "province": 4
                },
                {
                    "commutes": [
                        {
                            "proportion": 0.75,
                            "to": 1
                        },
                        {
                            "proportion": 0.5,
                            "to": 3
                        }
                    ],
                    "contactPools": [
                        {
                            "class": "Household",
                            "pools": []
                        },
                        {
                            "class": "Daycare",
                            "pools": []
                        },
                        {
                            "class": "PreSchool",
                            "pools": []
                        },
                        {
                            "class": "K12School",
                            "pools": []
                        },
                        {
                            "class": "College",
                            "pools": []
                        },
                        {
                            "class": "Workplace",
                            "pools": []
                        },
                        {
                            "class": "PrimaryCommunity",
                            "pools": []
                        },
                        {
                            "class": "SecondaryCommunity",
                            "pools": []
                        }
                    ],
                    "coordinate": {
                        "latitude": 0.0,
                        "longitude": 0.0
                    },
                    "id": 2,
                    "name": "Gent",
                    "population": 2500,
                    "province": 4
                },
                {
                    "commutes": [],
                    "contactPools": [
                        {
                            "class": "Household",
                            "pools": []
                        },
                        {
                            "class": "Daycare",
                            "pools": []
                        },
                        {
                            "class": "PreSchool",
                            "pools": []
                        },
                        {
                            "class": "K12School",
                            "pools": []
                        },
                        {
                            "class": "College",
                            "pools": []
                        },
                        {
                            "class": "Workplace",
                            "pools": []
                        },
                        {
                            "class": "PrimaryCommunity",
                            "pools": []
                        },
                        {
                            "class": "SecondaryCommunity",
                            "pools": []
                        }
                    ],
                    "coordinate": {
                        "latitude": 0.0,
                        "longitude": 0.0
                    },
                    "id": 3,
                    "name": "Mons",
                    "population": 2500,
                    "province": 4
                }
            ],
            "persons": []
        })";

        auto pop        = Population::Create();
        auto geoGridPtr = GetCommutesGeoGrid(pop.get());

        shared_ptr<stringstream> ss = make_shared<stringstream>();
        GeoGridJSONWriter        writer(ss);
        writer.Write(*geoGridPtr);

        EXPECT_TRUE(compareOutput(expectedOutput, ss->str()));
}

} // namespace
