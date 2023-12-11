// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_math.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <stddef.h>
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

auto solve_part1(const path& inputFile)
{
	auto image = readLines(inputFile);
	auto image2 = vector<string>();
	for(int y = 0; y < image.size(); ++y)
	{
		bool is_empty = all_of(begin(image[y]), end(image[y]), [](char a) { return a == '.'; });
		if(is_empty)
			image2.push_back(image[y]);
		image2.push_back(image[y]);
	}

	for(size_t x = 0; x < image2[0].length(); ++x)
	{
		bool all_empty = true;
		for(int y = 0; y < image2.size() && all_empty; ++y)
		{
			if(image2[y][x] != '.')
			{
				all_empty = false;
				break;
			}
		}
		if(all_empty)
		{
			for(int y = 0; y < image2.size(); ++y)
			{
				image2[y].insert(x, ".");
			}
			x++;
		}
	}
	image = image2;

	vector<vec2i> galaxis;
	int sum{};

	for(int y = 0; y < image.size(); ++y)
	{
		for(int x = 0; x < image[0].length(); ++x)
		{
			cout << image[y][x];
			if(image[y][x] == '#')
			{
				galaxis.push_back(vec2i{x, y});
			}
		}
	}

	auto distance = [](vec2i p, vec2i q) { return tuple_manhattan(p, q); };

	for(int i = 0; i < galaxis.size(); ++i)
	{
		for(int j = 0; j < i; ++j)
		{
			auto n = distance(galaxis[i], galaxis[j]);
			sum += n;
		}
	}

	return sum;
}

auto solve_part2(const path& inputFile, int64_t mult)
{
	auto image = readLines(inputFile);
	vector<vec2<int64_t>> galaxis;
	unordered_map<int64_t, int64_t> x_map;
	unordered_map<int64_t, int64_t> y_map;

	{
		int64_t y{};
		for(int yi = 0; yi < image.size(); ++yi)
		{
			y_map[yi] = y;
			bool is_empty =
			    all_of(begin(image[yi]), end(image[yi]), [](char a) { return a == '.'; });
			if(is_empty)
			{
				y += mult;
			} else
			{
				y++;
			}
		}

		int64_t x{};
		for(size_t xi = 0; xi < image[0].length(); ++xi)
		{
			x_map[xi] = x;
			bool all_empty = true;
			for(int yi = 0; yi < image.size() && all_empty; ++yi)
			{
				if(image[yi][xi] != '.')
				{
					all_empty = false;
					break;
				}
			}
			if(all_empty)
			{
				x += mult;
			} else
			{
				x++;
			}
		}

		for(int y = 0; y < image.size(); ++y)
		{
			for(int x = 0; x < image[0].length(); ++x)
			{
				if(image[y][x] == '#')
				{
					galaxis.push_back(vec2i{x, y});
				}
			}
		}
	}

	auto distance = [](vec2i p, vec2i q) { return tuple_manhattan(p, q); };
	int64_t sum{};
	for(int i = 0; i < galaxis.size(); ++i)
	{
		for(int j = 0; j < i; ++j)
		{
			auto g1 = galaxis[i];
			auto g2 = galaxis[j];
			auto g1prim = vec2<int64_t>(x_map[get<0>(g1)], y_map[get<1>(g1)]);
			auto g2prim = vec2<int64_t>(x_map[get<0>(g2)], y_map[get<1>(g2)]);

			auto n = distance(g1prim, g2prim);
			sum += n;
		}
	}

	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 374);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt"), 10) == 1030);
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt"), 100) == 8410);
}

int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);

	auto r1 = solve_part1(path(dataDir) / path("input.txt"));
	cout << dayName << " - part 1: " << r1 << "\n";

	auto r2 = solve_part2(path(dataDir) / path("input.txt"), 1000000);
	cout << dayName << " - part 2: " << r2 << "\n";

	return result;
}
