// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>
#include <common/tuple_math.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include <stdlib.h>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

// Part 1
using point_t = vec2<long long>;
using map_t = unordered_map<point_t, string>;

auto parseLine(const string& line)
{
	regex re(R"(([LRUD]) (\d+) \([#]([0-9a-f]+)\))");
	smatch m;
	regex_match(line, m, re);
	return make_tuple(m[1].str()[0], stoi(m[2].str()), m[3].str());
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
	point_t p;
	map_t map;
	point_t min_p;
	point_t max_p;

	auto data = parseLines(readLines(inputFile));
	for(auto& [dc, n, c] : data)
	{
		point_t d;
		switch(dc)
		{
			case 'D':
				d = point_t{0, 1};
				break;
			case 'U':
				d = point_t{0, -1};
				break;
			case 'R':
				d = point_t{1, 0};
				break;
			case 'L':
				d = point_t{-1, 0};
				break;
			default:
				break;
		}
		for(int i = 0; i < n; i++)
		{
			map[p] = c;
			p += d;

			min_p = {min(get<0>(min_p), get<0>(p)), min(get<1>(min_p), get<1>(p))};
			max_p = {max(get<0>(max_p), get<0>(p)), max(get<1>(max_p), get<1>(p))};
		}
	}

	map_t filled_map = map;
	for(int y = get<1>(min_p); y <= get<1>(max_p); ++y)
	{
		bool inside = false;
		string color;
		for(int x = get<0>(min_p); x <= get<0>(max_p);)
		{
			point_t p{x, y};
			if(map.contains(p))
			{
				inside = !inside;
				color = map[{x, y}];
				++x;
				if(map.contains({x, y}))
				{
					bool above = map.contains({x - 1, y - 1});
					bool under = map.contains({x - 1, y + 1});
					while(map.contains({x, y}))
					{
						color = map[{x, y}];
						++x;
					}
					if((map.contains({x - 1, y - 1}) && above) ||
					   (map.contains({x - 1, y + 1}) && under))
					{
						inside = !inside;
					}
				}
			} else
			{
				if(inside)
				{
					filled_map[{x, y}] = color;
				}
				++x;
			}
		}
	}

	return filled_map.size();
}

// Part 2

auto parseLine2(const string& line)
{
	regex re(R"([LRUD] \d+ \([#]([0-9a-f]{5})([0-3])\))");
	smatch m;
	regex_match(line, m, re);

	return make_pair(stoi(m[2].str()), stoi(m[1].str(), nullptr, 16));
}

auto parseLines2(const vector<string>& lines)
{
	vector<decltype(parseLine2(declval<string>()))> parsedLines;
	for(auto& line : lines)
	{
		parsedLines.push_back(parseLine2(line));
	}
	return parsedLines;
}

long long polygonPerimeter(const vector<point_t>& p)
{
	long long perimeter{};
	auto n = p.size();
	for(int i = 0; i < p.size(); ++i)
	{
		auto a = p[i % n];
		auto b = p[(i + 1) % n];
		auto ab = b - a;
		auto l = abs(get_x(ab)) + abs(get_y(ab));
		perimeter += l;
	}
	return perimeter;
}

long long polygonArea(const vector<point_t>& p)
{
	long long area{};
	auto n = p.size();
	for(int i = 0; i < p.size(); ++i)
	{
		auto a = p[i % n];
		auto b = p[(i + 1) % n];
		area += (get_y(a) + get_y(b)) * (get_x(a) - get_x(b));
	}
	area = abs(area / 2);
	auto perimeter = polygonPerimeter(p);
	return area + perimeter / 2 + 1;
}

auto solve_part2(const path& inputFile)
{
	auto data = parseLines2(readLines(inputFile));

	point_t p;
	map_t map;
	vector<point_t> polygon;
	for(auto& [dc, n] : data)
	{
		polygon.push_back(p);
		point_t d;
		switch(dc)
		{
			case 0:
				d = point_t{1, 0};
				break;
			case 1:
				d = point_t{0, 1};
				break;
			case 2:
				d = point_t{-1, 0};
				break;
			case 3:
				d = point_t{0, -1};
				break;
			default:
				break;
		}
		p += n * d;
	}
	return polygonArea(polygon);
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 62);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	auto p1 = vector<point_t>({{0, 0}, {4, 0}, {4, 2}, {0, 2}});
	REQUIRE(polygonArea(p1) == 15LL);

	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 952408144115LL);
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
