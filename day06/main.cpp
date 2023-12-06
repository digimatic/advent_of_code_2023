// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <filesystem>
#include <iostream>
#include <stddef.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

auto solve_part1(const path& inputFile)
{
	auto lines = readLines(inputFile);
	auto ts = extractIntegers(lines[0]);
	auto ds = extractIntegers(lines[1]);
	int sum{1};
	for(int i = 0; i < ts.size(); ++i)
	{
		int wins{};
		for(int t = 0; t <= ts[i]; t++)
		{
			auto travel_time = ts[i] - t;
			auto speed = t;
			auto dist = speed * travel_time;

			if(dist > ds[i])
			{
				wins++;
			}
		}
		sum *= wins;
	}
	return sum;
}

auto solve_part2(const path& inputFile)
{
	auto lines = readLines(inputFile);

	string time_s;
	string dist_s;
	for(auto d : lines[0])
	{
		if(d >= '0' && d <= '9')
		{
			time_s += d;
		}
	}

	for(auto d : lines[1])
	{
		if(d >= '0' && d <= '9')
		{
			dist_s += d;
		}
	}
	auto total_time = stoll(time_s);
	auto total_dist = stoll(dist_s);

	long long wins{};
	for(long long t = 0; t <= total_time; t++)
	{
		auto travel_time = total_time - t;
		auto speed = t;
		auto dist = speed * travel_time;

		if(dist > total_dist)
		{
			wins++;
		}
	}

	return wins;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 288);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 71503);
}

int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);

	auto r1 = solve_part1(path(dataDir) / path("input.txt"));
	cout << dayName << " - part 1: " << r1 << "\n";

	auto r2 = solve_part2(path(dataDir) / path("input.txt"));
	cout << dayName << " - part 2: " << r2 << "\n";

	return result;
}
