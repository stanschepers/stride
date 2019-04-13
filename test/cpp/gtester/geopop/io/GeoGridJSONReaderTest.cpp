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
 *  Copyright 2018, Jan Broeckhove and Bistromatics group.
 */

#include <gtest/gtest.h>

#include "GeoGridIOUtils.h"
#include "contact/ContactType.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/io/GeoGridJSONReader.h"
#include "pop/Population.h"

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;

namespace {

void getGeoGridFromJSON(const string& jsonString, shared_ptr<Population> pop)
{
        auto instream = make_unique<istringstream>(jsonString);

        GeoGridJSONReader geoGridJSONReader(move(instream), pop.get());

        geoGridJSONReader.Read();
}

void runPeopleTest(shared_ptr<Population> pop)
{
        auto& geoGrid  = pop->RefGeoGrid();
        auto  location = geoGrid[0];

        string cpTypes[8] = {"Household", "K12School",        "College",
                             "Workplace", "PrimaryCommunity", "SecondaryCommunity"};

        EXPECT_EQ(location->GetID(), 1);
        EXPECT_EQ(location->GetName(), "Bavikhove");
        EXPECT_EQ(location->GetProvince(), 4);
        EXPECT_EQ(location->GetPopCount(), 2500);
        EXPECT_EQ(get<0>(location->GetCoordinate()), 0);
        EXPECT_EQ(get<1>(location->GetCoordinate()), 0);

        vector<ContactPool*> pools;
        for (Id typ : IdList) {
                for (const auto& p : location->RefPools(typ)) {
                        pools.emplace_back(p);
                }
        }

        unsigned int contacPoolCounter = 0;
        for (const auto& pool : pools) {
                auto person = *(pool->begin());
                EXPECT_EQ(cpTypes[contacPoolCounter++], ToString(pool->GetType()));
                EXPECT_EQ(person->GetId(), 0);
                EXPECT_EQ(person->GetAge(), 18);
                EXPECT_EQ(person->GetPoolId(Id::K12School), 1);
                EXPECT_EQ(person->GetPoolId(Id::College), 1);
                EXPECT_EQ(person->GetPoolId(Id::Household), 1);
                EXPECT_EQ(person->GetPoolId(Id::Workplace), 1);
                EXPECT_EQ(person->GetPoolId(Id::PrimaryCommunity), 1);
                EXPECT_EQ(person->GetPoolId(Id::SecondaryCommunity), 1);
        }
}

TEST(GeoGridJSONReaderTest, readLocationsTest)
{
        string jsonString = R"(
        {
          "locations": [
            {
              "id": "1",
              "name": "Bavikhove",
              "province": "4",
              "population": "2500",
              "coordinate": {
                "longitude": "0",
                "latitude": "0"
              },
              "contactPools": ""
            },
            {
              "id": "2",
              "name": "Gent",
              "province": "3",
              "population": "5000",
              "coordinate": {
                "longitude": "0",
                "latitude": "0"
              },
              "contactPools": ""
            },
            {
              "id": "3",
              "name": "Mons",
              "province": "2",
              "population": "2500",
              "coordinate": {
                "longitude": "0",
                "latitude": "0"
              },
              "contactPools": ""
            }
          ],
          "persons": ""
        }
        )";

        auto pop = Population::Create();
        getGeoGridFromJSON(jsonString, pop);
        auto& geoGrid = pop->RefGeoGrid();

        map<unsigned int, shared_ptr<Location>> locations;
        locations[geoGrid[0]->GetID()] = geoGrid[0];
        locations[geoGrid[1]->GetID()] = geoGrid[1];
        locations[geoGrid[2]->GetID()] = geoGrid[2];

        const auto location1 = locations[1];
        const auto location2 = locations[2];
        const auto location3 = locations[3];

        EXPECT_EQ(location1->GetID(), 1);
        EXPECT_EQ(location1->GetName(), "Bavikhove");
        EXPECT_EQ(location1->GetProvince(), 4);
        EXPECT_EQ(location1->GetPopCount(), 2500);
        EXPECT_EQ(get<0>(location1->GetCoordinate()), 0);
        EXPECT_EQ(get<1>(location1->GetCoordinate()), 0);

        EXPECT_EQ(location2->GetID(), 2);
        EXPECT_EQ(location2->GetName(), "Gent");
        EXPECT_EQ(location2->GetProvince(), 3);
        EXPECT_EQ(location2->GetPopCount(), 5000);
        EXPECT_EQ(get<0>(location2->GetCoordinate()), 0);
        EXPECT_EQ(get<1>(location2->GetCoordinate()), 0);

        EXPECT_EQ(location3->GetID(), 3);
        EXPECT_EQ(location3->GetName(), "Mons");
        EXPECT_EQ(location3->GetProvince(), 2);
        EXPECT_EQ(location3->GetPopCount(), 2500);
        EXPECT_EQ(get<0>(location3->GetCoordinate()), 0);
        EXPECT_EQ(get<1>(location3->GetCoordinate()), 0);
}

TEST(GeoGridJSONReaderTest, readCommutesTest)
{

        string jsonString = R"(
        {
            "locations": [
                {
                    "id": "3",
                    "name": "Mons",
                    "province": "4",
                    "population": "2500",
                    "coordinate": {
                        "longitude": "0",
                        "latitude": "0"
                    }
                },
                {
                    "id": "2",
                    "name": "Gent",
                    "province": "4",
                    "population": "2500",
                    "coordinate": {
                        "longitude": "0",
                        "latitude": "0"
                    },
                    "commutes": [
                        {
                            "to": 1,
                            "proportion": 0.75
                        },
                        {
                            "to": 3,
                            "proportion": 0.5
                        }
                    ]
                },
                {
                    "id": "1",
                    "name": "Bavikhove",
                    "province": "4",
                    "population": "2500",
                    "coordinate": {
                        "longitude": "0",
                        "latitude": "0"
                    },
                    "commutes": [
                        {
                            "to": 2,
                            "proportion": 0.5
                        },
                        {
                            "to": 3,
                            "proportion": 0.25
                        }
                    ]
                }
            ],
            "persons": ""
        }
        )";

        auto pop = Population::Create();
        getGeoGridFromJSON(jsonString, pop);
        auto& geoGrid = pop->RefGeoGrid();

        map<unsigned int, shared_ptr<Location>> locations;

        locations[geoGrid[0]->GetID()] = geoGrid[0];
        locations[geoGrid[1]->GetID()] = geoGrid[1];
        locations[geoGrid[2]->GetID()] = geoGrid[2];

        auto location1 = locations[1];
        auto location2 = locations[2];
        auto location3 = locations[3];

        auto sortLoc = [](vector<pair<Location*, double>> loc) {
                sort(begin(loc), end(loc), [](const pair<Location*, double>& a, const pair<Location*, double>& b) {
                        return a.first->GetID() < b.first->GetID();
                });
                return loc;
        };

        {
                auto commuting_in  = sortLoc(location1->CRefIncomingCommutes());
                auto commuting_out = sortLoc(location1->CRefOutgoingCommutes());
                EXPECT_EQ(commuting_in.size(), 1);
                EXPECT_EQ(commuting_out.size(), 2);

                EXPECT_EQ(commuting_in[0].first->GetID(), 2);
                EXPECT_DOUBLE_EQ(commuting_in[0].second, 0.75);

                EXPECT_EQ(commuting_out[0].first->GetID(), 2);
                EXPECT_DOUBLE_EQ(commuting_out[0].second, 0.50);
                EXPECT_EQ(commuting_out[1].first->GetID(), 3);
                EXPECT_DOUBLE_EQ(commuting_out[1].second, 0.25);
        }
        {
                auto commuting_in  = sortLoc(location2->CRefIncomingCommutes());
                auto commuting_out = sortLoc(location2->CRefOutgoingCommutes());
                EXPECT_EQ(commuting_out.size(), 2);
                EXPECT_EQ(commuting_in.size(), 1);

                EXPECT_EQ(commuting_in[0].first->GetID(), 1);
                EXPECT_DOUBLE_EQ(commuting_in[0].second, 0.50);

                EXPECT_EQ(commuting_out[0].first->GetID(), 1);
                EXPECT_DOUBLE_EQ(commuting_out[0].second, 0.75);
                EXPECT_EQ(commuting_out[1].first->GetID(), 3);
                EXPECT_DOUBLE_EQ(commuting_out[1].second, 0.5);
        }
        {
                auto commuting_in  = sortLoc(location3->CRefIncomingCommutes());
                auto commuting_out = sortLoc(location3->CRefOutgoingCommutes());
                EXPECT_EQ(commuting_out.size(), 0);
                EXPECT_EQ(commuting_in.size(), 2);

                EXPECT_EQ(commuting_in[0].first->GetID(), 1);
                EXPECT_DOUBLE_EQ(commuting_in[0].second, 0.25);
                EXPECT_EQ(commuting_in[1].first->GetID(), 2);
                EXPECT_DOUBLE_EQ(commuting_in[1].second, 0.5);
        }
}

TEST(GeoGridJSONReaderTest, readContactPoolsTest)
{

        string jsonString = R"(
        {
            "locations": [
                {
                    "id": "1",
                    "name": "Bavikhove",
                    "province": "4",
                    "population": "2500",
                    "coordinate": {
                        "longitude": "0",
                        "latitude": "0"
                    },
                    "contactPools": [
                        {
                            "class": "K12School",
                            "pools": [
                                {
                                    "id": 1,
                                    "people": ""
                                }
                            ]
                        },
                        {
                            "class": "PrimaryCommunity",
                            "pools": [
                                {
                                    "id": 2,
                                    "people": ""
                                }
                            ]
                        },
                        {
                            "class": "College",
                            "pools": [
                                {
                                    "id": 3,
                                    "people": ""
                                }
                            ]
                        },
                        {
                            "class": "Household",
                            "pools": [
                                {
                                    "id": 4,
                                    "people": ""
                                }
                            ]
                        },
                        {
                            "class": "Workplace",
                            "pools": [
                                {
                                    "id": 5,
                                    "people": ""
                                }
                            ]
                        }
                    ]
                }
            ],
            "persons": ""
        }
        )";

        auto pop = Population::Create();
        getGeoGridFromJSON(jsonString, pop);
        auto& geoGrid  = pop->RefGeoGrid();
        auto  location = geoGrid[0];

        vector<ContactPool*> pools;

        for (Id typ : IdList) {
                for (const auto& p : location->CRefPools(typ)) {
                        pools.emplace_back(p);
                }
        }

        map<Id, bool> found = {{Id::K12School, false},
                               {Id::PrimaryCommunity, false},
                               {Id::College, false},
                               {Id::Household, false},
                               {Id::Workplace, false}};

        for (unsigned int i = 0; i < 5; i++) {
                EXPECT_FALSE(found[pools[i]->GetType()]);
                found[pools[i]->GetType()] = true;
        }
        for (auto& type : found) {
                EXPECT_TRUE(type.second);
        }
}

TEST(GeoGridJSONReaderTest, readPeopleTest)
{

        string jsonString = R"(
        {
          "locations": [
            {
              "id": "1",
              "name": "Bavikhove",
              "province": "4",
              "population": "2500",
              "coordinate": {
                "longitude": "0",
                "latitude": "0"
              },
              "contactPools": [
                {
                  "class": "K12School",
                  "pools": [
                    {
                      "id": "2",
                      "people": [
                        "0"
                      ]
                    }
                  ]
                },
                {
                  "class": "PrimaryCommunity",
                  "pools": [
                    {
                      "id": "3",
                      "people": [
                        "0"
                      ]
                    }
                  ]
                },
                {
                  "class": "SecondaryCommunity",
                  "pools": [
                    {
                      "id": "7",
                      "people": [
                        "0"
                      ]
                    }
                  ]
                },
                {
                  "class": "College",
                  "pools": [
                    {
                      "id": "4",
                      "people": [
                        "0"
                      ]
                    }
                  ]
                },
                {
                  "class": "Household",
                  "pools": [
                    {
                      "id": "5",
                      "people": [
                        "0"
                      ]
                    }
                  ]
                },
                {
                  "class": "Workplace",
                  "pools": [
                    {
                      "id": "6",
                      "people": [
                        "0"
                      ]
                    }
                  ]
                }
              ]
            }
          ],
          "persons": [
            {
              "id": "0",
              "age": "18",
              "K12School": "2",
              "College": "4",
              "Household": "5",
              "Workplace": "6",
              "PrimaryCommunity": "3",
              "SecondaryCommunity": "7"
            }
          ]
        }
        )";

        auto pop = Population::Create();
        getGeoGridFromJSON(jsonString, pop);
        runPeopleTest(pop);
}

TEST(GeoGridJSONReaderTest, emptyFileTest)
{
        string jsonString = R"()";
        auto   pop        = Population::Create();
        EXPECT_THROW(getGeoGridFromJSON(jsonString, pop), runtime_error);
}

TEST(GeoGridJSONReaderTest, invalidTypesTest)
{

        string jsonString = R"(
        {
          "locations": [
            {
              "id": 1,
              "name": "Bavikhove",
              "province": 4,
              "population": 2500,
              "coordinate": {
                "longitude": 0,
                "latitude": 0
              },
              "contactPools": [
                {
                  "class": "K12School",
                  "pools": [
                    {
                      "id": 2,
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
                      "id": 3,
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
                      "id": 7,
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
                      "id": 4,
                      "people": [
                        0
                      ]
                    }
                  ]
                },
                {
                  "class": "Household",
                  "pools": [
                    {
                      "id": 5,
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
                      "id": 6,
                      "people": [
                        0
                      ]
                    }
                  ]
                }
              ]
            }
          ],
          "persons": [
            {
              "id": 0,
              "age": 18,
              "K12School": 2,
              "College": 4,
              "Household": 5,
              "Workplace": 6,
              "PrimaryCommunity": 3,
              "SecondaryCommunity": 7
            }
          ]
        }
        )";

        auto pop = Population::Create();
        getGeoGridFromJSON(jsonString, pop);
        runPeopleTest(pop);
}

TEST(GeoGridJSONReaderTest, invalidJSONTest)
{
        string jsonString = R"(

          "locations": [
            {
              "id": 1,
              "name": "Bavikhove",
              "province": 4,
              "population": 2500,
              "coordinate": {
                "longitude": 0,
                "latitude": 0
              },
              "contactPools": [
                {
                  "class": "K12School",
                  "pools": [
                    {
                      "id": 2,
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
                      "id": 3,
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
                      "id": 7,
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
                      "id": 4,
                      "people": [
                        0
                      ]
                    }
                  ]
                },
                {
                  "class": "Household",
                  "pools": [
                    {
                      "id": 5,
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
                      "id": 6,
                      "people": [
                        0
                      ]
                    }
                  ]
                }
              ]
            }
          ]
          "persons: [
            {
              "id": 0,
              "age": 18,
              "K12School": 2,
              "College": 4,
              "Household": 5,
              "Workplace": 6,
              "PrimaryCommunity": 3,
              "SecondaryCommunity": 7
            }
          ]
        }
        )";

        auto pop = Population::Create();
        EXPECT_THROW(getGeoGridFromJSON(jsonString, pop), runtime_error);
}
} // namespace
