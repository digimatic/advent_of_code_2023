// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>
#include <common/tuple_math.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

using map_t = vector<string>;
using visited_key_t = vec2<int64_t>;
using visited_t = unordered_set<visited_key_t>;
using point_t = vec2<int64_t>;

[[nodiscard]] auto map_width(const map_t& map) -> int64_t
{
	return map[0].length();
}

[[nodiscard]] auto map_height(const map_t& map) -> int64_t
{
	auto h = map.size();
	return h;
}

[[nodiscard]] auto read(const map_t& map, int x, int y)
{
	if(x < 0 || y < 0 || x >= map[0].length() || y >= map.size())
		return '#';

	return map[y][x];
}

[[nodiscard]] auto read(const map_t& map, vec2i p)
{
	return read(map, get<0>(p), get<1>(p));
}

void write(map_t& map, int x, int y, char c)
{
	if(x < 0 || y < 0 || x >= map[0].length() || y >= map.size())
		return;

	map[y][x] = c;
}

[[nodiscard]] vec2i find_point(const map_t& map, char c = 'S')
{
	auto w = map_width(map);
	auto h = map_height(map);
	for(int y = 0; y < h; y++)
	{
		for(int x = 0; x < w; x++)
		{
			auto p = vec2i{x, y};
			if(read(map, x, y) == c)
			{
				return p;
			}
		}
	}
	return vec2i{};
};

// void print(const map_t& map, visited_t& v, point_t minp, point_t maxp)
// {
// 	auto w = map_width(map);
// 	auto h = map_height(map);
// 	cout << "x: " << get<0>(minp) << " - " << get<0>(maxp) << endl;
// 	cout << "y: " << get<1>(minp) << " - " << get<1>(maxp) << endl;

// 	for(int y = get<1>(minp); y <= get<1>(maxp); y++)
// 	{
// 		for(int x = get<0>(minp); x <= get<0>(maxp); x++)
// 		{
// 			auto p = vec2i{x, y};
// 			if(v.contains(p))
// 			{
// 				cout << "O";
// 			} else
// 			{
// 				if((x % w) == 0 && (y % h) == 0)
// 					cout << '!';
// 				else
// 					cout << read(map, x, y);
// 			}
// 		}
// 		cout << "\n";
// 	}
// 	cout << endl;
// }

constexpr int64_t mod(int64_t x, int64_t N)
{
	return (x % N + N) % N;
}

vector<tuple<int, int64_t>> border_i;

int64_t visit_count(map_t map, int64_t n)
{
	int64_t w = map_width(map);
	int64_t h = map_height(map);

	visited_t v;
	auto start = find_point(map, 'S');
	v.insert(start);
	write(map, get<0>(start), get<1>(start), '.');

	// vec2<int64_t> minp{0, 0};
	// vec2<int64_t> maxp{w - 1, h - 1};
	bool border_reached = false;
	for(int i = 0; i < n; i++)
	{
		// cout << "step i=" << (i + 1) << endl;
		visited_t vn;
		for(auto p0 : v)
		{
			auto next = [&map, &vn, p0, w, h, i /*, &minp, &maxp*/](int64_t dx, int64_t dy) {
				int64_t x = get<0>(p0) + dx;
				int64_t y = get<1>(p0) + dy;
				bool b = read(map, mod(x, w), mod(y, w)) == '.';
				if(b)
				{
					int bn = border_i.size();
					if(x == (0 - bn * w) || y == (0 - bn * h) || x == ((bn + 1) * w - 1) ||
					   y == ((bn + h) * h - 1))
					{

						if(border_i.empty() || get<0>(border_i.back()) != (i + 1))
						{
							border_i.push_back(make_pair(i + 1, -1));
						}
					}
					// get<0>(minp) = min(get<0>(minp), x);
					// get<1>(minp) = min(get<1>(minp), y);

					// get<0>(maxp) = max(get<0>(maxp), x);
					// get<1>(maxp) = max(get<1>(maxp), y);

					vn.insert({x, y});
				}
			};

			next(-1, 0);
			next(1, 0);
			next(0, 1);
			next(0, -1);
		}
		v = vn;
		// cout << i + 1 << endl;
		// print(map, v, minp, maxp);
		if(!border_i.empty() && get<0>(border_i.back()) == (i + 1))
		{
			get<1>(border_i.back()) = v.size();
		}
	}

	auto sum = v.size();
	return sum;
}

auto solve_part1(const path& inputFile, int64_t n)
{
	auto map = readLines(inputFile);
	return visit_count(map, n);
}

auto solve_part2(const path& inputFile, int64_t n)
{
	auto map = readLines(inputFile);

	border_i.clear();
	visit_count(map, 400);
	int64_t fi = 0;
	for(fi = 0; fi < border_i.size() - 2; ++fi)
	{
		if(get<0>(border_i[fi + 1]) - get<0>(border_i[fi]) ==
		   get<0>(border_i[fi + 2]) - get<0>(border_i[fi + 1]))
			break;
	}
	auto first_len = get<0>(border_i[fi]);
	auto cycle_length = get<0>(border_i[fi + 1]) - get<0>(border_i[fi]);

	vector<int64_t> v;
	for(int64_t i = 0; i < 3; i++)
	{
		v.push_back(get<1>(border_i[i + fi]));
	}

	// solution to the equation
	// n = a i^2 + b i + c
	// maunal solving gives:
	// 2 a =    n1 - 2 n2 +   n3
	// 2 b = -3 n1 + 4 n2 -   n3
	// 2 c =                2 n3
	int64_t a2 = (v[0] - 2 * v[1] + v[2]);
	int64_t b2 = (-3 * v[0] + 4 * v[1] - v[2]);
	int64_t c2 = 2 * v[0];

	int64_t n_cycles = (n - first_len) / cycle_length - fi;
	auto f2 = (a2 * n_cycles * n_cycles + b2 * n_cycles + c2) / 2;

	n_cycles = (n - first_len) / cycle_length;
	f2 = (a2 * n_cycles * n_cycles + b2 * n_cycles + c2) / 2;

	return f2;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt"), 6) == 16);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	// REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt"), 6) == 16);
	// REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt"), 10) == 50);
	// REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt"), 50) == 1594);
	// REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt"), 100) == 6536);
	// REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt"), 500) == 167004);
	// REQUIRE(solve_part2(path(dataDir) / path("input.txt"), 26501365) == 625382480005896);
	// REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt"), 1000) == 668697);
	// REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt"), 5000) == 16733044);
}

int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);
	if(result != 0)
		return result;

	auto r1 = solve_part1(path(dataDir) / path("input.txt"), 64);
	cout << dayName << " - part 1: " << r1 << "\n";

	auto r2 = solve_part2(path(dataDir) / path("input.txt"), 26501365);
	cout << dayName << " - part 2: " << r2 << "\n";
	// assert(r2 == 625382480005896);

	return result;
}
