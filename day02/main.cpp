// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

auto solve_part1(const path& inputFile)
{
	int sum{};
	auto lines = readLines(inputFile);
	int gameId = 1;
	for(auto& l : lines)
	{
		auto c1 = l.find(':');
		auto sets_line = l.substr(c1 + 2);
		auto sets = split(regex("; "), sets_line);
		unordered_map<string, int> cubes;
		cubes["red"] = 0;
		cubes["green"] = 0;
		cubes["blue"] = 0;
		bool possible = true;
		for(auto subset_line : sets)
		{
			auto cubes_line = split(regex(", "), subset_line);
			for(auto cube : cubes_line)
			{
				regex re("(\\d+)\\s(\\w+)");
				smatch m;
				regex_match(cube, m, re);
				auto n = stoi(m[1].str());
				auto c = m[2].str();
				cubes[c] = n;
			}
			if(cubes["red"] > 12 || cubes["green"] > 13 || cubes["blue"] > 14)
			{
				possible = false;
				break;
			}
		}
		if(possible)
		{
			sum += gameId;
		}
		gameId++;
	}

	return sum;
}

auto solve_part2(const path& inputFile)
{
	int sum{};
	auto lines = readLines(inputFile);
	int gameId = 1;
	for(auto& l : lines)
	{
		auto c1 = l.find(':');
		auto sets_line = l.substr(c1 + 2);
		auto sets = split(regex("; "), sets_line);
		unordered_map<string, int> cubes;
		cubes["red"] = 0;
		cubes["green"] = 0;
		cubes["blue"] = 0;
		bool possible = true;
		for(auto subset_line : sets)
		{
			auto cubes_line = split(regex(", "), subset_line);
			for(auto cube : cubes_line)
			{
				regex re("(\\d+)\\s(\\w+)");
				smatch m;
				regex_match(cube, m, re);
				auto n = stoi(m[1].str());
				auto c = m[2].str();
				cubes[c] = max(n, cubes[c]);
			}
		}
		int red = cubes["red"];
		int green = cubes["green"];
		int blue = cubes["blue"];
		int power = red * green * blue;
		sum += power;
		gameId++;
	}

	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 8);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 2286);
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
