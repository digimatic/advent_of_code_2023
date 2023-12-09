// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <filesystem>
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using namespace std::string_literals;
using std::filesystem::path;

auto parseLine(const string& line)
{
	return extractIntegers<int64_t>(line, true);
}

auto parseLines(const vector<string>& lines)
{
	vector<decltype(parseLine(declval<string>()))> parsedLines;
	for(auto& line : lines)
	{
		parsedLines.push_back(parseLine(line));
	}
	return parsedLines;
}

auto solve_part1(const path& inputFile)
{
	int x{};
	int total_sum{};

	auto lines = parseLines(readLines(inputFile));

	for(auto& line : lines)
	{
		vector<vector<int64_t>> datas;
		datas.push_back(line);
		while(true)
		{
			{
				auto& data = datas[datas.size() - 1];
				vector<int64_t> data2;
				for(int i = 1; i < data.size(); ++i)
				{
					data2.push_back(data[i] - data[i - 1]);
				}
				datas.push_back(data2);
			}

			bool all_zero = true;
			for(auto v : datas[datas.size() - 1])
			{
				if(v != 0)
					all_zero = false;
			}
			if(all_zero)
				break;
		}
		int64_t sum{};
		for(auto& d : datas)
		{
			sum += d[d.size() - 1];
		}
		total_sum += sum;
	}

	return total_sum;
}

auto solve_part2(const path& inputFile)
{
	int x{};
	int total_sum{};

	auto lines = parseLines(readLines(inputFile));

	for(auto& line : lines)
	{
		vector<vector<int64_t>> datas;
		datas.push_back(line);
		while(true)
		{
			{
				auto& data = datas[datas.size() - 1];
				vector<int64_t> data2;
				for(int i = 1; i < data.size(); ++i)
				{
					data2.push_back(data[i] - data[i - 1]);
				}
				datas.push_back(data2);
			}

			bool all_zero = true;
			for(auto v : datas[datas.size() - 1])
			{
				if(v != 0)
					all_zero = false;
			}
			if(all_zero)
				break;
		}
		int64_t sum{};
		for(int i = datas.size() - 1; i >= 0; --i)
		{
			sum = datas[i][0] - sum;
		}
		total_sum += sum;
	}

	return total_sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 114);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 2);
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
