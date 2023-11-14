// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>

#include <algorithm>
#include <cassert>
#include <deque>
#include <filesystem>
#include <functional>
#include <iostream>
#include <regex>
#include <stddef.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using namespace std::string_literals;
using std::filesystem::path;

auto parseLine(const string& line)
{
	auto remaining = line;
	size_t taken = 0;
	auto n = stoi(remaining, &taken);
	return n;

	/*
	    regex re("(\\w+)\\)(\\w+)");
	    smatch m;
	    regex_match(line, m, re);
	    return {m[1].str(), m[2].str()};
	*/

	/*
	auto tokens = split(regex("\\s+"), line);
	return std::pair(tokens[0], tokens[1]);
	return tokens;
	*/

	//return extractIntegers(line, false);

}

auto parseLines(const vector<string>& lines)
{
	vector<decltype(parseLine(declval<string>()))> parsedLines;
	for(auto& line : lines)
	{
		parsedLines.push_back(parseLine(line));
	}
	return parsedLines;
}

auto solve_part1(const path& inputFile)
{
	int x{};
	int sum{};

	auto parsedData = parseLines(readLines(inputFile));
	for(auto& d : parsedData)
	{


	}

	auto lines = readLines(inputFile);
	for(int i = 0; i < lines.size();++i)
	{
		auto& line = lines[i];
		int v = stoi(line);
		if(line.substr(1) == "")
		{

		} else
		{

		}

	}

	return -1;
}

auto solve_part2(const path& inputFile)
{
	return -1;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == -1);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == -1);
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
