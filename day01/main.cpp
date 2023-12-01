// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>

#include <algorithm>
#include <cassert>
#include <deque>
#include <filesystem>
#include <functional>
#include <iostream>
#include <regex>
#include <set>
#include <stddef.h>
#include <stdexcept>
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

using namespace westerstrom;
using namespace std;
using namespace std::string_literals;
using std::filesystem::path;

auto solve_part1(const path& inputFile)
{
	int sum{0};

	auto lines = readLines(inputFile);
	for(int i = 0; i < lines.size(); ++i)
	{
		auto& line = lines[i];
		int f;
		int l;
		for(auto d : line)
		{
			if(d >= '0' && d <= '9')
			{
				f = d - '0';
				break;
			}
		}
		for(int j = line.size() - 1; j >= 0; j--)
		{
			char d = line[j];
			if(d >= '0' && d <= '9')
			{
				l = d - '0';
				break;
			}
		}
		int n = f * 10 + l;

		sum += n;
	}

	return sum;
}

auto solve_part2(const path& inputFile)
{
	int x{};
	int sum{0};

	const vector<string> numbers = {"one", "two",   "three", "four", "five",
	                                "six", "seven", "eight", "nine"};
	auto lines = readLines(inputFile);
	for(int i = 0; i < lines.size(); ++i)
	{
		auto& line = lines[i];
		int f = -1;
		int l = -1;
		int j = 0;
		for(auto d : line)
		{
			if(d >= '0' && d <= '9')
			{
				f = d - '0';
				break;
			} else
			{
				for(int k = 0; k < numbers.size(); k++)
				{
					if(line.substr(j).starts_with(numbers[k]))
					{
						f = k + 1;
						break;
					}
				}
			}
			if(f >= 0)
				break;
			j++;
		}

		for(int j = line.size() - 1; j >= 0; j--)
		{
			char d = line[j];
			if(d >= '0' && d <= '9')
			{
				l = d - '0';
				break;
			} else
			{
				auto s = line.substr(j);
				for(int k = 0; k < numbers.size(); k++)
				{
					auto nn = numbers[k];
					if(s.starts_with(nn))
					{
						l = k + 1;
						break;
					}
				}
			}
			if(l >= 0)
				break;
		}
		int n = f * 10 + l;

		sum += n;
	}

	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 142);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample2.txt")) == 281);
}

int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);

	cout << dayName << " - part 1: " << solve_part1(path(dataDir) / path("input.txt")) << "\n";
	cout << dayName << " - part 2: " << solve_part2(path(dataDir) / path("input.txt")) << "\n";
	return result;
}
