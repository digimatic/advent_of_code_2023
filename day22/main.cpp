// Advent of Code 2023
// Peter Westerström (digimatic)

// Day 22 note: Not very fast, requires release build to finish in half a minute.

#include "config.h"

#include <common/common.h>
#include <common/tuple_math.h>

#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <stddef.h>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

using point_t = vec3i;
using brick_t = pair<point_t, point_t>;
using bricks_t = vector<brick_t>;

[[nodiscard]] brick_t parseLine(const string& line)
{
	auto m = extractIntegers(line);
	return make_pair(make_tuple(m[0], m[1], m[2]), make_tuple(m[3], m[4], m[5]));
}

[[nodiscard]] bricks_t parseLines(const vector<string>& lines)
{
	vector<decltype(parseLine(declval<string>()))> parsedLines;
	for(auto& line : lines)
	{
		parsedLines.push_back(parseLine(line));
	}
	return parsedLines;
}

[[nodiscard]] brick_t move_down(const brick_t& brick)
{
	auto [px, py, pz] = brick.first;
	auto [qx, qy, qz] = brick.second;
	if(pz > 1 && qz > 1)
		return brick_t{{px, py, pz - 1}, {qx, qy, qz - 1z}};
	else
		return brick;
}

[[nodiscard]] constexpr bool collide_test(const brick_t& a, const brick_t& b)
{
	auto [pa, qa] = a;
	auto [pb, qb] = b;

	if(get<0>(qa) < get<0>(pb))
		return false;
	if(get<0>(qb) < get<0>(pa))
		return false;

	if(get<1>(qa) < get<1>(pb))
		return false;
	if(get<1>(qb) < get<1>(pa))
		return false;

	if(get<2>(qa) < get<2>(pb))
		return false;
	if(get<2>(qb) < get<2>(pa))
		return false;

	return true;
}

bool let_brick_fall_one(bricks_t& bricks, size_t i)
{
	auto falling_brick_0 = bricks[i];
	auto falling_brick = move_down(falling_brick_0);
	if(falling_brick_0 == falling_brick)
		return false;

	bool collided{};
	for(int j = 0; j < bricks.size(); ++j)
	{
		if(j == i)
			continue;
		collided = collide_test(falling_brick, bricks[j]);
		if(collided)
			break;
	}
	if(!collided)
	{
		bricks[i] = falling_brick;
	}
	return !collided;
}

int let_all_fall(bricks_t& bricks)
{
	bool some_moved = false;
	vector<bool> bricks_fall_status(bricks.size(), false); // used in part 2 only
	do
	{
		some_moved = false;
		for(int i = 0; i < bricks.size(); ++i)
		{
			auto moved = let_brick_fall_one(bricks, i);
			bricks_fall_status[i] = bricks_fall_status[i] || moved;
			some_moved = some_moved || moved;
		}
	} while(some_moved);
	// return for part 2
	return ranges::count_if(bricks_fall_status, [](auto b) { return b; });
}

[[nodiscard]] bool can_be_removed(bricks_t bricks, int i)
{
	bricks.erase(bricks.begin() + i);

	for(int i = 0; i < bricks.size(); ++i)
	{
		auto moved = let_brick_fall_one(bricks, i);
		if(moved)
			return false;
	}
	return true;
}

[[nodiscard]] int remove_one_reaction(bricks_t bricks, int i)
{
	bricks.erase(bricks.begin() + i);
	return let_all_fall(bricks);
}

[[nodiscard]] auto solve_part1(const path& inputFile)
{
	auto bricks = parseLines(readLines(inputFile));
	let_all_fall(bricks);

	int sum{};
	for(int i = 0; i < bricks.size(); ++i)
	{
		if(can_be_removed(bricks, i))
			sum++;
	}

	return sum;
}

[[nodiscard]] auto solve_part2(const path& inputFile)
{
	auto bricks = parseLines(readLines(inputFile));

	let_all_fall(bricks);

	int sum{};
	for(int i = 0; i < bricks.size(); ++i)
	{
		sum += remove_one_reaction(bricks, i);
	}

	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 5);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 7);
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
