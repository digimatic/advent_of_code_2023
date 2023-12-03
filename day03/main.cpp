// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>
#include <common/tuple_math.h>

#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <stddef.h>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using namespace std::string_literals;
using std::filesystem::path;

auto solve_part1(const path& inputFile)
{
	int sum{};
	auto map = readLines(inputFile);

	int w = map[0].length();
	int h = map.size();

	auto read = [&map, w, h](int x, int y) {
		if(x >= 0 && y >= 0 && x < w && y < h)
			return map.at(y).at(x);
		else
			return '.';
	};

	auto isdigit = [](char c) { return c >= '0' && c <= '9'; };

	for(int y = 0; y < map.size(); ++y)
	{
		auto& line = map[y];
		int x = 0;
		bool symbol = false;
		int start = -1;
		while(x < (w + 1))
		{
			auto c = read(x, y);
			if(c >= '0' && c <= '9')
			{
				if(start == -1)
					start = x;
				auto neigh = neighbors3x3(vec2i{x, y});
				for(auto p : neigh)
				{
					auto cc = read(get<0>(p), get<1>(p));
					if(isdigit(cc) || cc == '.')
					{

					} else
					{
						symbol = true;
					}
				}
			} else
			{
				if(symbol)
				{
					auto numberstr = line.substr(start, x - start);
					auto n = stoi(numberstr);
					sum += n;
				}
				start = -1;
				symbol = false;
			}
			++x;
		}
	}

	return sum;
}

auto solve_part2(const path& inputFile)
{
	int sum{};
	auto map = readLines(inputFile);

	int w = map[0].length();
	int h = map.size();

	auto read = [&map, w, h](int x, int y) {
		if(x >= 0 && y >= 0 && x < w && y < h)
			return map.at(y).at(x);
		else
			return '.';
	};

	auto isdigit = [](char c) { return c >= '0' && c <= '9'; };

	auto getnumberstart = [&read, &isdigit](vec2i p) {
		while(isdigit(read(get<0>(p), get<1>(p))))
		{
			get<0>(p)--;
		}
		get<0>(p)++;
		return p;
	};

	for(int y = 0; y < map.size(); ++y)
	{
		for(int x = 0; x < w; ++x)
		{
			auto c = read(x, y);
			if(c == '*')
			{
				unordered_set<vec2i> numbers;
				auto neigh = neighbors3x3(vec2i{x, y});
				for(auto p : neigh)
				{
					if(isdigit(read(get<0>(p), get<1>(p))))
					{
						auto start = getnumberstart(p);
						numbers.insert(start);
					}
				}
				if(numbers.size() == 2)
				{
					int n = 1;
					for(auto [xx, yy] : numbers)
					{
						auto x = stoi(map[yy].substr(xx));
						n *= x;
					}
					sum += n;
				}
			}
		}
	}

	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 4361);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 467835);
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
