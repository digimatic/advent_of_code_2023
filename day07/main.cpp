// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <algorithm>
#include <cassert>
#include <ctype.h>
#include <filesystem>
#include <functional>
#include <iostream>
#include <iterator>
#include <stdint.h>
#include <string_view>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

// Input parsing

auto parseLine(const string& line)
{
	auto hand = line.substr(0, 5);
	auto bid = stoi(line.substr(6));
	return make_pair(hand, bid);
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

// Part 1

int64_t card_value(char c)
{
	if(isdigit(c))
	{
		return c - '0';
	} else if(c == 'T')
	{
		return 10;
	} else if(c == 'J')
	{
		return 11;
	} else if(c == 'Q')
	{
		return 12;
	} else if(c == 'K')
	{
		return 13;
	} else if(c == 'A')
	{
		return 14;
	}
	throw -1;
}

auto solve_part1(const path& inputFile)
{
	vector<pair<int, int64_t>> hand_wins;
	auto parsedData = parseLines(readLines(inputFile));
	int i = 0;
	for(auto [hand, bid] : parsedData)
	{
		unordered_map<char, int> counts;
		int64_t hand_value{0};
		for(auto card : hand)
		{
			counts[card] += 1;
			hand_value *= 100;
			auto cv = card_value(card);
			hand_value += cv;
		}
		// 1 00 00 00 00 00
		vector<int> vals;
		for(auto [c, v] : counts)
		{
			vals.push_back(v);
		}
		sort(begin(vals), end(vals), std::greater<>());

		int64_t p{};
		if(vals[0] == 5)
		{
			p = 21;
		} else if(vals[0] == 4)
		{
			p = 20;
		} else if(vals[0] == 3 && vals[1] == 2)
		{
			p = 19;
		} else if(vals[0] == 3)
		{
			p = 18;
		} else if(vals[0] == 2 && vals[1] == 2)
		{
			p = 17;
		} else if(vals[0] == 2)
		{
			p = 16;
		} else if(vals.size() == 5)
		{
			p = 15;
		}
		p *= 10000000000;
		hand_value += p;
		hand_wins.emplace_back(bid, hand_value);

		++i;
	}
	sort(begin(hand_wins), end(hand_wins), [](auto& a, auto& b) { return a.second > b.second; });

	int sum{};
	for(int i = 0; i < hand_wins.size(); ++i)
	{
		int64_t rank = hand_wins.size() - i;
		int64_t bid = hand_wins[i].first;
		auto v = bid * rank;
		sum += v;
	}

	return sum;
}

// Part 2

int64_t card_value2(char c)
{
	if(isdigit(c))
	{
		return c - '0';
	} else if(c == 'T')
	{
		return 10;
	} else if(c == 'J')
	{
		return 1;
	} else if(c == 'Q')
	{
		return 12;
	} else if(c == 'K')
	{
		return 13;
	} else if(c == 'A')
	{
		return 14;
	}
	throw -1;
}

auto solve_part2(const path& inputFile)
{
	vector<pair<int, int64_t>> hand_wins;
	auto parsedData = parseLines(readLines(inputFile));
	int i = 0;
	for(auto [hand, bid] : parsedData)
	{
		unordered_map<char, int> counts;
		int64_t hand_value{0};
		int j{};
		for(auto card : hand)
		{
			if(card == 'J')
			{
				j++;
			} else
			{
				counts[card] += 1;
			}
			hand_value *= 100;
			auto cv = card_value2(card);
			hand_value += cv;
		}
		// 1 00 00 00 00 00
		vector<int> vals;

		for(auto [c, v] : counts)
		{
			vals.push_back(v);
		}

		int64_t p{};
		if(vals.size() == 0) // J J J J J
		{
			p = 21;
			assert(j == 5);
		} else
		{
			sort(begin(vals), end(vals), std::greater<>());

			if(j == 0) // no J
			{
				if(vals[0] == 5)
				{
					p = 21;
				} else if(vals[0] == 4)
				{
					p = 20;
				} else if(vals[0] == 3 && vals[1] == 2)
				{
					p = 19;
				} else if(vals[0] == 3)
				{
					p = 18;
				} else if(vals[0] == 2 && vals[1] == 2)
				{
					p = 17;
				} else if(vals[0] == 2)
				{
					p = 16;
				} else if(vals.size() == 5)
				{
					p = 15;
				}
			} else if(j == 1) // x x x x J
			{
				if(vals[0] >= 4) // x x x x J => five of a kind
				{
					p = 21;
				} else if(vals[0] >= 3) // x x x y J => 4 of a kind
				{
					p = 20;
				} else if(vals[0] == 2 && vals[1] == 2) // xx yy J
				{
					p = 19;
				} else if(vals[0] == 2) // x x y z J => 3 of a kind
				{
					p = 18;
				} else // 4 different + J => 1 pair
				{
					p = 16;
				}
			} else if(j == 2) // x x x J J
			{
				if(vals[0] >= 3) // x x x J J
				{
					p = 21;
				} else if(vals[0] == 2) // x x y J J => 4 of a kind
				{
					p = 20;
				} else // x y z J J => three of a kind
				{
					p = 18;
				}

			} else if(j == 3) // x x J J J
			{
				if(vals[0] == 2) // x x J J J => five of a kind
				{
					p = 21;
				} else // x y J J J => 4 of a kind
				{
					p = 20;
				}
			} else if(j == 4) // x J J J J => 5 of a kind
			{
				p = 21;
			}
		}
		p *= 10000000000;
		hand_value += p;
		hand_wins.emplace_back(bid, hand_value);

		++i;
	}
	sort(begin(hand_wins), end(hand_wins), [](auto& a, auto& b) { return a.second > b.second; });

	int sum{};
	for(int i = 0; i < hand_wins.size(); ++i)
	{
		int64_t rank = hand_wins.size() - i;
		int64_t bid = hand_wins[i].first;
		auto v = bid * rank;
		sum += v;
	}

	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 6440);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 5905);
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
