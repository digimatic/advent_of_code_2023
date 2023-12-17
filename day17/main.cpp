// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>

#include <filesystem>
#include <iostream>
#include <queue>
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
using state_t = tuple<int, int, int, int, int, int>;  // x y dx dy steps heat_sum
using visited_key_t = tuple<int, int, int, int, int>; // x y dx dy steps
using visited_t = unordered_set<visited_key_t>;

[[nodiscard]] auto map_width(const map_t& map)
{
	return map[0].length();
}

[[nodiscard]] auto map_height(const map_t& map)
{
	auto h = map.size();
	return h;
}

struct state_greater
{
	bool operator()(const state_t& t1, const state_t& t2)
	{
		return get<5>(t1) > get<5>(t2);
	};
};

template <class CanGoForwardT, class CanTurnT, class GoalTestT>
auto solve(const map_t& map, CanGoForwardT can_forward, CanTurnT can_turn, GoalTestT at_goal)
{
	auto w = map_width(map);
	auto h = map_height(map);

	visited_t v;
	priority_queue<state_t, vector<state_t>, state_greater> q;
	q.emplace(0, 0, 1, 0, 0, 0);
	q.emplace(0, 0, 0, 1, 0, 0);
	while(!q.empty())
	{
		auto s = q.top();
		q.pop();
		auto [x, y, dx, dy, n, heat_sum] = s;

		auto k = make_tuple(x, y, dx, dy, n);
		if(v.contains(k))
			continue;
		v.insert(k);

		if(at_goal(x, y, n))
		{
			return heat_sum;
		}

		x += dx;
		y += dy;
		n++;
		if(x >= 0 && y >= 0 && x < w && y < h)
		{
			heat_sum += int(map[y][x] - '0');
			if(can_forward(n))
			{
				q.emplace(x, y, dx, dy, n, heat_sum);
			}
			if(can_turn(n))
			{
				q.emplace(x, y, dy, dx, 0, heat_sum);
				q.emplace(x, y, -dy, -dx, 0, heat_sum);
			}
		}
	}
	return -1;
}

auto solve_part1(const path& inputFile)
{
	auto map = readLines(inputFile);
	auto w = map_width(map);
	auto h = map_height(map);
	auto can_forward = [](int n) { return n < 3; };
	auto can_turn = [](int n) { return true; };
	auto at_goal = [w, h](int x, int y, int n) { return x == (w - 1) && y == (h - 1); };
	return solve(map, can_forward, can_turn, at_goal);
}

auto solve_part2(const path& inputFile)
{
	auto map = readLines(inputFile);
	auto w = map_width(map);
	auto h = map_height(map);
	auto can_forward = [](int n) { return n < 10; };
	auto can_turn = [](int n) { return n >= 4; };
	auto at_goal = [w, h](int x, int y, int n) { return x == (w - 1) && y == (h - 1) && n >= 4; };
	return solve(map, can_forward, can_turn, at_goal);
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 102);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 94);
	REQUIRE(solve_part2(path(dataDir) / path("inputExample2.txt")) == 71);
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
