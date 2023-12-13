// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using namespace std::string_literals;
using std::filesystem::path;

auto parseLines(const vector<string>& lines)
{
	vector<string> current;
	vector<vector<string>> maps;
	for(auto& line : lines)
	{
		if(line.empty())
		{
			maps.push_back(current);
			current.clear();
		} else
		{
			current.push_back(line);
		}
	}
	if(!current.empty())
		maps.push_back(current);
	return maps;
}

auto map_width(const vector<string>& map)
{
	return map[0].length();
}

auto map_height(const vector<string>& map)
{
	auto h = map.size();
	return h;
}
auto read(const vector<string>& map, int x, int y)
{
	if(x < 0 || y < 0 || x >= map[0].length() || y >= map.size())
		return '.';

	return map[y][x];
}

auto solve_part1(const path& inputFile)
{
	int sum1{};
	int sum2{};
	auto maps = parseLines(readLines(inputFile));

	for(auto& map : maps)
	{
		auto w = map_width(map);
		auto h = map_height(map);

		for(int x = 0; x < w - 1; x++)
		{
			bool all_equal = true;
			int i;
			for(i = 0; i < w && all_equal; i++)
			{
				int x1 = x - i;
				int x2 = x + 1 + i;
				if(x1 < 0 || x2 >= w)
					break;
				for(int y = 0; y < h; y++)
				{
					if(read(map, x1, y) != read(map, x2, y))
					{
						all_equal = false;
						break;
					}
				}
				if(!all_equal)
					break;
			}

			if(all_equal)
			{
				sum1 += x + 1;
			}
		}

		for(int y = 0; y < h - 1; y++)
		{
			bool all_equal = true;
			int i;
			for(i = 0; i < h && all_equal; i++)
			{
				int y1 = y - i;
				int y2 = y + 1 + i;
				if(y1 < 0 || y2 >= h)
					break;
				for(int x = 0; x < w; x++)
				{
					if(read(map, x, y1) != read(map, x, y2))
					{
						all_equal = false;
						break;
					}
				}
				if(!all_equal)
					break;
			}

			if(all_equal)
			{
				sum2 += y + 1;
			}
		}
	}

	auto sum = sum1 + 100 * sum2;
	return sum;
}

auto solve_part2(const path& inputFile)
{
	int sum1{};
	int sum2{};
	auto maps = parseLines(readLines(inputFile));

	for(auto& map : maps)
	{
		auto w = map_width(map);
		auto h = map_height(map);

		for(int x = 0; x < w - 1; x++)
		{
			int n_not_equal = 0;
			int i;
			for(i = 0; i < w; i++)
			{
				int x1 = x - i;
				int x2 = x + 1 + i;
				if(x1 < 0 || x2 >= w)
					break;
				for(int y = 0; y < h; y++)
				{
					if(read(map, x1, y) != read(map, x2, y))
					{
						n_not_equal++;
					}
				}
			}

			if(n_not_equal == 1)
			{
				sum1 += x + 1;
			}
		}

		for(int y = 0; y < h - 1; y++)
		{
			int n_not_equal = 0;
			int i;
			for(i = 0; i < h; i++)
			{
				int y1 = y - i;
				int y2 = y + 1 + i;
				if(y1 < 0 || y2 >= h)
					break;
				for(int x = 0; x < w; x++)
				{
					if(read(map, x, y1) != read(map, x, y2))
					{
						n_not_equal++;
					}
				}
			}

			if(n_not_equal == 1)
			{
				sum2 += y + 1;
			}
		}
	}

	auto sum = sum1 + 100 * sum2;
	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 405);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 400);
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
