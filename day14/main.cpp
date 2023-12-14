// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/vector_hash.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <stdint.h>
#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

using map_t = vector<string>;

auto map_width(const map_t& map)
{
	return map[0].length();
}

auto map_height(const map_t& map)
{
	auto h = map.size();
	return h;
}
auto read(const map_t& map, int x, int y)
{
	if(x < 0 || y < 0 || x >= map[0].length() || y >= map.size())
		return '#';

	return map[y][x];
}

void write(map_t& map, int x, int y, char c)
{
	if(x < 0 || y < 0 || x >= map[0].length() || y >= map.size())
		return;

	map[y][x] = c;
}

void tilt_north(map_t& map)
{
	auto w = map_height(map);
	auto h = map_height(map);

	for(int xi = 0; xi < w; xi++)
	{
		for(int yi = 0; yi < h; ++yi)
		{
			if(read(map, xi, yi) == 'O')
			{
				int yi2 = yi - 1;
				while(yi2 >= 0 && read(map, xi, yi2) == '.')
				{
					yi2--;
				}
				yi2++;
				if(yi2 != yi)
				{
					write(map, xi, yi, '.');
					write(map, xi, yi2, 'O');
				}
			}
		}
	}
}

void tilt_south(map_t& map)
{
	auto w = map_height(map);
	auto h = map_height(map);

	for(int xi = 0; xi < w; xi++)
	{
		for(int yi = h - 1; yi >= 0; --yi)
		{
			if(read(map, xi, yi) == 'O')
			{
				int yi2 = yi + 1;
				while(yi2 < h && read(map, xi, yi2) == '.')
				{
					yi2++;
				}
				yi2--;
				if(yi2 != yi)
				{
					write(map, xi, yi, '.');
					write(map, xi, yi2, 'O');
				}
			}
		}
	}
}

void tilt_west(map_t& map)
{
	auto w = map_height(map);
	auto h = map_height(map);

	for(int yi = 0; yi < h; yi++)
	{
		for(int xi = 0; xi < w; ++xi)
		{
			if(read(map, xi, yi) == 'O')
			{
				int xi2 = xi - 1;
				while(xi2 >= 0 && read(map, xi2, yi) == '.')
				{
					xi2--;
				}
				xi2++;
				if(xi2 != xi)
				{
					write(map, xi, yi, '.');
					write(map, xi2, yi, 'O');
				}
			}
		}
	}
}

void tilt_east(map_t& map)
{
	auto w = map_height(map);
	auto h = map_height(map);

	for(int yi = 0; yi < h; yi++)
	{
		for(int xi = w - 1; xi >= 0; --xi)
		{
			if(read(map, xi, yi) == 'O')
			{
				int xi2 = xi + 1;
				while(xi2 < w && read(map, xi2, yi) == '.')
				{
					xi2++;
				}
				xi2--;
				if(xi2 != xi)
				{
					write(map, xi, yi, '.');
					write(map, xi2, yi, 'O');
				}
			}
		}
	}
}


void print_map(map_t& map)
{
	cout << "\n";
	for(auto& l : map)
	{
		cout << l << "\n";
	}
}
int get_total_load(map_t& map)
{
	int sum{};
	auto h = map_height(map);
	for(int yi = 0; yi < h; yi++)
	{
		auto n = count(begin(map[yi]), end(map[yi]), 'O');
		auto yv = h - yi;
		auto v = n * yv;
		sum += v;
	}
	return sum;
}

auto solve_part1(const path& inputFile)
{
	auto map = readLines(inputFile);
	tilt_north(map);
	return get_total_load(map);
}

auto solve_part2(const path& inputFile)
{
	auto map = readLines(inputFile);
	unordered_map<map_t, int, VectorHash> seen;
	bool do_rest = false;
	for(int64_t i = 0; i < 1000000000; ++i)
	{
		tilt_north(map);
		tilt_west(map);
		tilt_south(map);
		tilt_east(map);

		if(!do_rest)
		{
			if(seen.contains(map))
			{
				auto seen_i = seen[map];
				auto cycle_len = i - seen_i;
				int n_left = (1000000000- i) / cycle_len;
				i += n_left * cycle_len;
				do_rest = true;
			}
			seen[map] = i;
		}
	}

	return get_total_load(map);
}


TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 136);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 64);
}

int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);
	if(result!=0)
		return result;

	auto r1 = solve_part1(path(dataDir) / path("input.txt"));
	cout << dayName << " - part 1: " << r1 << "\n";

	auto r2 = solve_part2(path(dataDir) / path("input.txt"));
	cout << dayName << " - part 2: " << r2 << "\n";

	return result;
}
