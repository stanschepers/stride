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

#include "geopop/io/EpiOutputHDF5Writer.h"
#include "contact/AgeBrackets.h"
#include "disease/Health.h"
#include "geopop/GeoGrid.h"
#include "geopop/io/H5Utils.h"
#include "util/FileSys.h"

#include <gtest/gtest.h>
#include <vector>

namespace {

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::util;
using namespace H5;
using namespace H5Utils;

vector<double> expected_percentages = {0.2,
                                       0.5,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.5,
                                       0.0,
                                       0.1,
                                       1.0,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.2,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.5,
                                       0.5,
                                       0.1,
                                       0.0,
                                       1.0,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.28,
                                       0.0,
                                       0.35714285714285715,
                                       0.0,
                                       0.0,
                                       0.35714285714285715,
                                       0.2857142857142857,
                                       0.12,
                                       0.8333333333333334,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.0,
                                       0.16666666666666666};

void validateDayAttributes(unsigned int day_num, unsigned long location_count, const Group& day)
{
        unsigned int  read_day_num;
        unsigned long read_location_count;

        ReadAttribute(read_day_num, "day_num", day);
        ReadAttribute(read_location_count, "location_count", day);

        EXPECT_EQ(day_num, read_day_num);
        EXPECT_EQ(location_count, read_location_count);
}

void validateLocationAttributes(double lat, double lon, string name, unsigned int id, unsigned int province,
                                unsigned int pop_count, unsigned long size, const DataSet& location)
{
        double        read_lat;
        double        read_lon;
        string        read_name;
        unsigned int  read_id;
        unsigned int  read_province;
        unsigned int  read_pop_count;
        unsigned long read_size;

        ReadAttribute(read_lat, "latitude", location);
        ReadAttribute(read_lon, "longitude", location);
        ReadAttribute(read_name, "name", location);
        ReadAttribute(read_id, "id", location);
        ReadAttribute(read_province, "province", location);
        ReadAttribute(read_pop_count, "population", location);
        ReadAttribute(read_size, "size", location);

        EXPECT_EQ(read_lat, lat);
        EXPECT_EQ(read_lon, lon);
        EXPECT_EQ(read_name, name);
        EXPECT_EQ(read_id, id);
        EXPECT_EQ(read_province, province);
        EXPECT_EQ(read_pop_count, pop_count);
        EXPECT_EQ(read_size, size);
}

void validateEpiData(vector<double> expected, const DataSet& location)
{
        vector<H5EpiOutput> actual(42);
        location.read(actual.data(), GetCompoundType<H5EpiOutput>());

        for (size_t i = 0; i < ageBrackets.size(); ++i) {
                for (size_t j = 0; j < healthStatuses.size(); ++j) {
                        unsigned int idx = (i * healthStatuses.size()) + j;
                        EXPECT_EQ(i, actual[idx].age_bracket);
                        EXPECT_EQ(j, actual[idx].health_status);
                        EXPECT_EQ(expected[idx], actual[idx].percentage);
                }
        }
}

TEST(EpiOutputHDF5WriterTest, zeroLocationsTest)
{
        auto geoGrid = GeoGrid<Epidemiologic>(nullptr);

        EpiOutputHDF5Writer writer(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/zeroLocationsTest.h5");

        writer.Update(geoGrid, 0);
        writer.Write();

        H5File file(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/zeroLocationsTest.h5", H5F_ACC_RDONLY);

        unsigned int read_day_count;
        ReadAttribute(read_day_count, "day_count", file);
        EXPECT_EQ(1, read_day_count);

        Group day0(file.openGroup("/DAY0"));
        validateDayAttributes(0, 0, day0);
}

TEST(EpiOutputHDF5WriterTest, oneLocationZeroDaysTest)
{
        EpiOutputHDF5Writer writer(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/zeroDaysTest.h5");

        writer.Write();

        H5File file(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/zeroDaysTest.h5", H5F_ACC_RDONLY);

        unsigned int read_day_count;
        ReadAttribute(read_day_count, "day_count", file);
        EXPECT_EQ(0, read_day_count);
}

TEST(EpiOutputHDF5WriterTest, oneLocationOneDayTest)
{
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

        EpiOutputHDF5Writer writer(FileSys::GetTestsDir().string() +
                                   "/testdata/EpiOutputHDF5/oneLocationOneDayTest.h5");

        writer.Update(geoGrid, 0);
        writer.Write();

        H5File file(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/oneLocationOneDayTest.h5",
                    H5F_ACC_RDONLY);

        unsigned int read_day_count;
        ReadAttribute(read_day_count, "day_count", file);
        EXPECT_EQ(1, read_day_count);

        Group day0(file.openGroup("/DAY0"));
        validateDayAttributes(0, 1, day0);

        DataSet loc0(day0.openDataSet("LOC0"));
        validateLocationAttributes(0.0, 0.0, "test", 1, 1, 0, 42, loc0);

        validateEpiData(expected_percentages, loc0);
}

TEST(EpiOutputHDF5WriterTest, twoLocationsSixDayTest)
{
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

        EpiOutputHDF5Writer writer(FileSys::GetTestsDir().string() +
                                   "/testdata/EpiOutputHDF5/twoLocationsFiveDaysTest.h5");

        for (unsigned int i = 0; i < 51; i += 10)
                writer.Update(geoGrid, i);
        writer.Write();

        H5File file(FileSys::GetTestsDir().string() + "/testdata/EpiOutputHDF5/twoLocationsFiveDaysTest.h5",
                    H5F_ACC_RDONLY);

        unsigned int read_day_count;
        ReadAttribute(read_day_count, "day_count", file);
        EXPECT_EQ(6, read_day_count);

        for (unsigned int i = 0; i < 6; ++i) {
                Group day(file.openGroup("/DAY" + to_string(i)));
                validateDayAttributes(i * 10, 2, day);

                DataSet loc0(day.openDataSet("LOC0"));
                DataSet loc1(day.openDataSet("LOC1"));

                validateLocationAttributes(0.0, 0.0, "test0", 1, 1, 0, 42, loc0);
                validateLocationAttributes(1.0, 1.0, "test1", 1, 1, 100, 42, loc1);

                validateEpiData(expected_percentages, loc0);
        }
}

} // namespace
