// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <regex>
#include <stdint.h>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

auto parseLine(const string& line)
{
	auto tokens = split(regex(","), line);
	return tokens;
}

tuple<string, char, int> parseToken(const string& token)
{
	regex re(R"(([a-z]+)([=-])(\d*))");
	smatch m;
	regex_match(token, m, re);
	auto ls = m[3].str();
	int l{};
	if(ls.length() > 0)
	{
		l = stoi(ls);
	}
	return make_tuple(m[1].str(), m[2].str()[0], l);
}

int hash_of(string s)
{
	int v{};
	for(auto c : s)
	{
		v += int(c);
		v *= 17;
		v %= 256;
	}
	return v;
}

auto solve_part1(const path& inputFile)
{
	auto tokens = parseLine(readLines(inputFile)[0]);
	int sum{};
	for(auto token : tokens)
	{
		sum += hash_of(token);
	}
	return sum;
}

using box_t = tuple<string, int>;

auto solve_part2(const path& inputFile)
{
	auto tokens = parseLine(readLines(inputFile)[0]);

	unordered_map<int, vector<box_t>> boxes;
	for(auto& token : tokens)
	{
		auto [label, op, focal_len] = parseToken(token);
		int box_id = hash_of(label);
		auto& box = boxes[box_id];
		if(op == '-')
		{
			auto it =
			    remove_if(begin(box), end(box), [&label](auto& l) { return get<0>(l) == label; });
			box.erase(it, end(box));
		} else if(op == '=')
		{
			auto it =
			    find_if(begin(box), end(box), [&label](auto& l) { return get<0>(l) == label; });
			if(it != end(box))
			{
				(*it) = box_t{label, focal_len};
			} else
			{
				box.emplace_back(label, focal_len);
			}
		}
	}

	int64_t sum{};
	for(auto& [box_id, lenses] : boxes)
	{
		int64_t slot = 1;
		for(auto& [label, focal] : lenses)
		{
			int64_t v = (box_id + 1) * slot * focal;
			sum += v;
			slot++;
		}
	}

	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(hash_of("HASH") == 52);
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 1320);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 145);
}

int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);
	if(result != 0)
		return result;

	auto r1 = solve_part1(path(dataDir) / path("input.txt"));
	cout << dayName << " - part 1: " << r1 << "\n";

	auto r2 = solve_part2(path(dataDir) / path("input.txt"));
	cout << dayName << " - part 2: " << r2 << "\n";

	return result;
}
