// Advent of Code 2023
// Peter Westerström (digimatic)

// Day 23 note: Brute force. Slow slow, takes 9 minutes on my computer.

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>
#include <common/tuple_math.h>
#include <common/vector_hash.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stack>
#include <stddef.h>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_set>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

using map_t = vector<string>;
using visited_key_t = vec2i;
using visited_t = unordered_set<visited_key_t>;
using state_t = tuple<vec2i, visited_t>;

[[nodiscard]] auto map_height(const map_t& map) -> int64_t
{
	return map.size();
}

[[nodiscard]] auto read(const map_t& map, const vec2i& p)
{
	return map[get<1>(p)][get<0>(p)];
}

unsigned long find_longest_path(const map_t& map)
{
	auto h = map_height(map);

	stack<state_t> q;
	q.emplace(vec2i{1, 0}, visited_t{});
	visited_t longest;
	while(!q.empty())
	{
		auto s = q.top();
		q.pop();

		auto [p, v] = s;
		if(get<1>(p) == h)
		{
			cout << "Found path with " << v.size() - 1 << " steps.";
			if(v.size() > longest.size())
			{
				cout << "  - new longest" << endl;
				longest = v;
			} else
			{
				cout << endl;
			}
			continue;
		}
		if(get<1>(p) < 0)
			continue;
		auto c = read(map, p);
		if(c == '#')
			continue;
		if(v.contains(p))
			continue;
		v.insert(p);
		bool take_step{false};
		switch(c)
		{
			case '>':
				p = p + vec2i{1, 0};
				break;
			case '<':
				p = p + vec2i{-1, 0};
				break;
			case '^':
				p = p + vec2i{0, -1};
				break;
			case 'v':
				p = p + vec2i{0, 1};
				break;
			default:
				take_step = true;
				break;
		}
		if(!take_step)
		{
			q.emplace(p, v);
			continue;
		}

		auto next = [&q, &v](vec2i p, vec2i d) {
			p = p + d;
			q.emplace(p, v);
		};

		next(p, {1, 0});
		next(p, {-1, 0});
		next(p, {0, 1});
		next(p, {0, -1});
	}
	return longest.size() - 1;
}

int find_longest_path_part2(const map_t& map, visited_t& v, const vec2i& p)
{
	auto h = map_height(map);
	if(get<1>(p) == h)
		return 0;
	if(get<1>(p) < 0)
		return -1;
	auto c = read(map, p);
	if(c == '#')
		return -1;
	if(v.contains(p))
		return -1;
	v.insert(p);
	auto n1 = find_longest_path_part2(map, v, p + vec2i{1, 0});
	auto n2 = find_longest_path_part2(map, v, p + vec2i{-1, 0});
	auto n3 = find_longest_path_part2(map, v, p + vec2i{0, -1});
	auto n4 = find_longest_path_part2(map, v, p + vec2i{0, 1});
	auto m = max(max(max(n1, n2), n3), n4);
	v.erase(p);
	return m == -1 ? -1 : 1 + m;
}

auto solve_part1(const path& inputFile)
{
	auto map = readLines(inputFile);
	return find_longest_path(map);
}

auto solve_part2(const path& inputFile)
{
	auto map = readLines(inputFile);
	for(auto& row : map)
	{
		for(auto& c : row)
		{
			switch(c)
			{
				case '>':
					[[fallthrough]];
				case '<':
					[[fallthrough]];
				case '^':
					[[fallthrough]];
				case 'v':
					c = '.';
					break;
				default:
					break;
			}
		}
	}
	visited_t v{};
	return find_longest_path_part2(map, v, vec2i{1, 0}) - 1;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 94);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 154);
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
