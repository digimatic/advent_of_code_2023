// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>
#include <common/tuple_math.h>
#include <common/vector_hash.h>

#include <algorithm>
#include <cassert>
#include <deque>
#include <filesystem>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <regex>
#include <set>
#include <stddef.h>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#ifdef PARALLEL
#include <execution>
    const auto selected_policy = std::execution::par;
#endif

using namespace westerstrom;
using namespace std;
using namespace std::string_literals;
using std::filesystem::path;

using map_t = vector<string>;

[[nodiscard]] auto map_width(const map_t& map)
{
	return map[0].length();
}

[[nodiscard]] auto map_height(const map_t& map)
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

void write(map_t& map, int x, int y, char c)
{
	if(x < 0 || y < 0 || x >= map[0].length() || y >= map.size())
		return;

	map[y][x] = c;
}

using dirs_t = unordered_set<char>;
using visited_t = unordered_map<tuple<int, int>, dirs_t>;
using pos_dir_t = tuple<vec2i, char>;

[[nodiscard]] vec2i step(vec2i p, char d)
{
	switch(d)
	{
		case 'U':
			return {get<0>(p), get<1>(p) - 1};
		case 'D':
			return {get<0>(p), get<1>(p) + 1};
		case 'L':
			return {get<0>(p) - 1, get<1>(p)};
		case 'R':
			return {get<0>(p) + 1, get<1>(p)};
	}
	std::unreachable();
}

[[nodiscard]] pair<char, char> new_dirs(const map_t& map, vec2i p, char d)
{
	char c = read(map, get<0>(p), get<1>(p));
	switch(c)
	{
		case '.':
			return {d, d};
		case '/':
			if(d == 'L')
				return {'D', 'D'};
			else if(d == 'R')
				return {'U', 'U'};
			else if(d == 'U')
				return {'R', 'R'};
			else
				return {'L', 'L'};
		case '\\':
			if(d == 'L')
				return {'U', 'U'};
			else if(d == 'R')
				return {'D', 'D'};
			else if(d == 'U')
				return {'L', 'L'};
			else
				return {'R', 'R'};
		case '-':
			if(d == 'L')
				return {'L', 'L'};
			else if(d == 'R')
				return {'R', 'R'};
			else if(d == 'U')
				return {'L', 'R'};
			else
				return {'L', 'R'};
			break;
		case '|':
			if(d == 'L')
				return {'D', 'U'};
			else if(d == 'R')
				return {'D', 'U'};
			else if(d == 'U')
				return {'U', 'U'};
			else
				return {'D', 'D'};
	}
	std::unreachable();
}

pair<pos_dir_t, pos_dir_t> next(const map_t& map, char d, vec2i p)
{
	char c = read(map, get<0>(p), get<1>(p));
	vec2i p1;
	vec2i p2;

	auto [d1, d2] = new_dirs(map, p, d);
	p1 = step(p, d1);
	p2 = step(p, d2);
	return {{p1, d1}, {p2, d2}};
}

auto energize(const map_t& map, int start_x, int start_y, char start_d, bool verbose = false)
{
	int w = map_width(map);
	int h = map_height(map);
	auto is_inside = [w, h](vec2i p) {
		auto x = get<0>(p);
		auto y = get<1>(p);
		return x >= 0 && y >= 0 && x < w && y < h;
	};

	visited_t visited;
	dirs_t right;
	right.insert(start_d);
	visited[make_tuple(start_x, start_y)] = right;

	auto print = [&map, &visited, w, h]() {
		for(int y = 0; y < h; ++y)
		{
			for(int x = 0; x < w; x++)
			{
				auto& v = visited[make_tuple(x, y)];
				auto n = v.size();
				if(n == 0)
				{
					cout << '.';
				} else if(n == 1)
				{
					auto c = *(v.begin());
					switch(c)
					{
						case 'U':
							c = '^';
							break;
						case 'D':
							c = 'v';
							break;
						case 'L':
							c = '<';
							break;
						case 'R':
							c = '>';
							break;
						default:
							break;
					}
					cout << c;
				} else
				{
					cout << to_string(n);
				}
			}
			cout << "\n";
		}
		cout << "\n";
	};

	while(true)
	{
		if(verbose)
			print();
		auto visited_next = visited;

		for(auto [p, dirs] : visited)
		{
			for(auto d : dirs)
			{
				auto [pos_dir_2, pos_dir_3] = next(map, d, p);
				auto [pos2, dir2] = pos_dir_2;
				if(is_inside(pos2))
				{
					auto& v = visited_next[pos2];
					v.insert(dir2);
				}
				auto [pos3, dir3] = pos_dir_3;
				if(is_inside(pos3))
				{
					auto& v = visited_next[pos3];
					v.insert(dir3);
				}
			}
		}

		if(visited_next == visited)
		{
			break;
		}
		visited = visited_next;
	}

	int sum{};
	for(auto [_, dirs] : visited)
	{
		if(dirs.size() > 0)
			sum++;
	}

	return sum;
}

auto solve_part1(const path& inputFile)
{
	auto map = readLines(inputFile);
	return energize(map, 0, 0, 'R');
}

auto solve_part2(const path& inputFile)
{
	auto map = readLines(inputFile);
	auto w = map_width(map);
	auto h = map_height(map);

#ifdef PARALLEL
	vector<int> results(2 * w + 2 * h);
	iota(begin(results), end(results), 0);
	for_each_n(selected_policy, begin(results), int(w),
	           [&map, &results](auto& x) { results[x] = energize(map, x, 0, 'D'); });
	cout << "Bottom Row\n";
	for_each_n(selected_policy, begin(results) + w, int(w),
	           [&map, &results, w, h](auto x) { results[x] = energize(map, x - w, h - 1, 'U'); });
	cout << "Left Columnl\n";
	for_each_n(selected_policy, begin(results) + 2 * w, int(h),
	           [&map, &results, w, h](auto y) { results[y] = energize(map, 0, y - 2 * w, 'R'); });
	cout << "Right Column\n";
	for_each_n(
	    selected_policy, begin(results) + 2 * w + h, int(h),
	    [&map, &results, w, h](auto y) { results[y] = energize(map, w - 1, y - 2 * w - h, 'L'); });
	auto it = max_element(begin(results), end(results));
	return *it;
#else
	int max_sum{};
	cout << "Top Row\n";
	for(int x = 0; x < w; x++)
	{
		max_sum = max(max_sum, energize(map, x, 0, 'D'));
	}
	cout << "Bottom Row\n";
	for(int x = 0; x < w; x++)
	{
		max_sum = max(max_sum, energize(map, x, h - 1, 'U'));
	}
	cout << "Left Column\n";
	for(int y = 0; y < h; y++)
	{
		max_sum = max(max_sum, energize(map, 0, y, 'R'));
	}
	cout << "Right Column\n";
	for(int y = 0; y < h; y++)
	{
		max_sum = max(max_sum, energize(map, w - 1, y, 'L'));
	}
	return max_sum;
#endif
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 46);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 51);
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
