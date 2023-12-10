// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>
#include <common/tuple_math.h>

#include <filesystem>
#include <iostream>
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

auto solve_part12(const path& inputFile)
{
	int sum{};
	auto map = readLines(inputFile);
	auto h = map.size();
	auto w = map[0].length();
	auto read = [&map, w, h](vec2i p) {
		if(get<0>(p) < 0 || get<1>(p) < 0 || get<0>(p) >= w || get<1>(p) >= h)
			return '.';
		return map[get<1>(p)][get<0>(p)];
	};

	auto find_point = [&map, w, h, read](char c = 'S') {
		for(int y = 0; y < h; y++)
		{
			for(int x = 0; x < w; x++)
			{
				auto p = vec2i{x, y};
				if(read(p) == c)
				{
					return p;
				}
			}
		}
		return vec2i{};
	};

	auto next = [&map, w, h, read](vec2i p0, vec2i p) {
		auto pu = vec2i{get<0>(p), get<1>(p) - 1};
		auto pd = vec2i{get<0>(p), get<1>(p) + 1};
		auto pl = vec2i{get<0>(p) - 1, get<1>(p)};
		auto pr = vec2i{get<0>(p) + 1, get<1>(p)};
		auto cl = read(pl);
		auto cr = read(pr);
		auto cu = read(pu);
		auto cd = read(pd);
		auto c = read(p);

		if(pu != p0 && (cu == 'S' || cu == '|' || cu == '7' || cu == 'F') &&
		   (c == 'S' || c == '|' || c == 'L' || c == 'J'))
		{
			return pu;
		}

		if(pd != p0 && (cd == 'S' || cd == '|' || cd == 'J' || cd == 'L') &&
		   (c == 'S' || c == '|' || c == 'F' || c == '7'))
		{
			return pd;
		}

		if(pl != p0 && (cl == 'S' || cl == '-' || cl == 'F' || cl == 'L') &&
		   (c == 'S' || c == '-' || c == 'J' || c == '7'))
		{
			return pl;
		}

		if(pr != p0 && (cr == 'S' || cr == '-' || cr == 'J' || cr == '7') &&
		   (c == 'S' || c == '-' || c == 'F' || c == 'L'))
		{
			return pr;
		}

		throw -1;
	};

	vec2i start = find_point('S');
	vec2i p = start;
	vec2i p0{-2, -2};
	int step_count{};
	vec2i start1;
	vec2i end1;
	unordered_set<vec2i> map_path;
	map_path.insert(p);
	do
	{
		auto p1 = next(p0, p);
		p0 = p;
		p = p1;
		if(step_count == 0)
			start1 = p1;
		step_count++;
		map_path.insert(p);

		if(p == start)
		{
			end1 = p0;
			break;
		}
	} while(true);

	// step_count/2 is answer for part1.
	// part 2 continues

	// replace the S with the correct tile.
	auto s1 = start1 - start;
	auto s2 = end1 - start;
	if(s1 == vec2i{-1, 0} && s2 == vec2i{0, -1} || s1 == vec2i{0, -1} && s2 == vec2i{-1, 0})
	{
		map[get<1>(start)][get<0>(start)] = 'J';
	} else if(s1 == vec2i{1, 0} && s2 == vec2i{0, -1} || s1 == vec2i{0, -1} && s2 == vec2i{1, 0})
	{
		map[get<1>(start)][get<0>(start)] = 'L';
	} else if(s1 == vec2i{-1, 0} && s2 == vec2i{0, 1} || s1 == vec2i{0, 1} && s2 == vec2i{-1, 0})
	{
		map[get<1>(start)][get<0>(start)] = '7';
	} else if(s1 == vec2i{1, 0} && s2 == vec2i{0, 1} || s1 == vec2i{0, 1} && s2 == vec2i{1, 0})
	{
		map[get<1>(start)][get<0>(start)] = 'F';
	}

	const unordered_set<char> horizontals = {'|', '7', 'L', 'F', 'J'};

	auto is_inside = [&map, &map_path, w, h, read, horizontals](vec2i p) {
		int x = get<0>(p);
		int y = get<1>(p);
		char c0 = '.';
		bool even_walls = true;
		for(++x; x < w; ++x)
		{
			auto c = read(vec2i{x, y});
			if(map_path.contains(vec2i{x, y}) && horizontals.contains(c))
			{
				if(c == '|')
				{
					even_walls = !even_walls;
				} else
				{
					if((c0 == 'F' && c == '7'))
					{
						even_walls = !even_walls;
						c0 = '.';
					} else if((c0 == 'L' && c == 'J'))
					{
						even_walls = !even_walls;
						c0 = '.';
					} else if((c0 == 'L' && c == '7'))
					{
						c0 = '.';
					} else if((c0 == 'F' && c == 'J'))
					{
						c0 = '.';
					} else
					{
						even_walls = !even_walls;
						c0 = c;
					}
				}
			}
		}
		return (!even_walls);
	};

	for(int y = 0; y < h; y++)
	{
		for(int x = 0; x < w; x++)
		{
			auto p = vec2i{x, y};
			if(map_path.contains(p))
			{
				cout << '*';
			} else
			{
				bool inside = is_inside(p);
				if(inside)
				{
					sum++;
					cout << 'I';

				} else
				{
					cout << 'O';
				}
			}
		}
		cout << "\n";
	}
	cout << "\n";
	return make_pair(step_count / 2, sum);
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part12(path(dataDir) / path("inputExample1.txt")).first == 8);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part12(path(dataDir) / path("inputExample2.txt")).second == 8);
	REQUIRE(solve_part12(path(dataDir) / path("inputExample3.txt")).second == 10);
}

int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);

	auto r1 = solve_part12(path(dataDir) / path("input.txt")).first;
	cout << dayName << " - part 1: " << r1 << "\n";

	auto r2 = solve_part12(path(dataDir) / path("input.txt")).second;
	cout << dayName << " - part 2: " << r2 << "\n";

	return result;
}
