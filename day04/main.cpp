// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <regex>
#include <string_view>
#include <string>
#include <unordered_map>
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
	auto tokens = split(regex("\\:|\\|"), line);
	auto part1 = tokens[1];
	auto part2 = tokens[2];
	auto w = extractIntegers(part1);
	auto h = extractIntegers(part2);
	return make_pair(w, h);
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
	int sum{};
	auto parsedData = parseLines(readLines(inputFile));
	for(auto& [wl, hl] : parsedData)
	{
		int points{};
		int p = 1;
		for(auto h : hl)
		{
			if(find(begin(wl), end(wl), h) != end(wl))
			{
				points = p;
				p = p * 2;
			}
		}
		sum += points;
	}
	return sum;
}

auto solve_part2(const path& inputFile)
{
	int sum{};
	unordered_map<int, int> cards;
	auto parsedData = parseLines(readLines(inputFile));
	int card_num = 1;
	for(auto& [wl, hl] : parsedData)
	{
		int points{};
		cards[card_num]++;
		for(auto h : hl)
		{
			if(find(begin(wl), end(wl), h) != end(wl))
			{
				points++;
			}
		}

		int n = cards[card_num];
		for(int c = 0; c < points; c++)
		{
			cards[card_num + c + 1] += n;
		}

		card_num++;
	}

	for(auto [c, n] : cards)
	{
		sum += n;
	}

	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 13);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 30);
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
