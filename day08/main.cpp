// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <iterator>
#include <stdint.h>
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <ranges>
#include <stddef.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "main.h"
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

auto solve_part1(const path& inputFile)
{
	auto lines = readLines(inputFile);
	auto instructions = lines[0];
	unordered_map<string, pair<string, string>> map;
	string target = "ZZZ";
	string current = "AAA";
	for(int i = 2; i < lines.size(); ++i)
	{
		auto& line = lines[i];
		auto src = line.substr(0, 3);
		auto left = line.substr(7, 3);
		auto r = line.substr(12, 3);
		map[src] = make_pair(left, r);

		if(i == lines.size() - 1)
		{
			// target = src;
		}
		if(i == 2)
		{
			// current = src;
		}
	}

	int64_t sum{};
	size_t i{};
	while(current != target)
	{
		char s = instructions[i];
		if(s == 'L')
		{
			current = map[current].first;
		} else
		{
			current = map[current].second;
		}
		sum++;
		i++;
		if(i >= instructions.size())
		{
			i = 0;
		}
	}
	return sum;
}

int64_t run_process(std::string current, std::string target, const std::string& instructions,
                    std::unordered_map<std::string, std::pair<std::string, std::string>>& map)
{
	int64_t i = 0;
	int64_t sum = 0;

	return sum;
}
auto solve_part2(const path& inputFile)
{

	int x{};

	auto lines = readLines(inputFile);
	auto instructions = lines[0];
	unordered_map<string, pair<string, string>> map;
	vector<string> current;
	for(int i = 2; i < lines.size(); ++i)
	{
		auto& line = lines[i];
		auto src = line.substr(0, 3);
		auto left = line.substr(7, 3);
		auto r = line.substr(12, 3);
		map[src] = make_pair(left, r);

		if(src[2] == 'A')
		{
			current.push_back(src);
		}
	}

	vector<vector<int64_t>> steps;

	for(auto c : current)
	{
		unordered_map<string, int64_t> targets;
		int64_t i = 0;
		int64_t sum{};
		while(true)
		{
			char s = instructions[i];

			auto p = map[c];
			if(s == 'L')
			{
				c = p.first;
			} else
			{
				c = p.second;
			}
			sum++;

			if(c[2] == 'Z')
			{
				if(targets.contains(c))
				{
					break;
				}
				targets[c] = sum;
			}

			i++;
			if(i >= instructions.size())
			{
				i = 0;
			}
		}
		vector<int64_t> s;
		for(auto [_, n] : targets)
		{

			s.push_back(n);
		}
		steps.push_back(s);
	}

	vector<int64_t> steps0;
	std::transform(begin(steps), end(steps), back_inserter(steps0), [](const auto& v) {
		assert(v.size() == 1); // it turns out there is only one ending (one Z).
		return v.at(0);
	});

#ifdef __clang__
	// fold_left_first is not yet in clang. hard code for now..
	auto common_factor = 407;
#else
	auto common_factor =
	    std::ranges::fold_left_first(steps0, [](auto a, auto b) { return std::gcd(a, b); }).value();
	cout << "Common factor: " << common_factor << "\n";
#endif

	int64_t p{1};
	cout << "Number of steps:\n";
	for(auto& v : steps)
	{
		cout << v[0] << "\n";
		p *= (v[0] / common_factor);
	}
	cout << "\n";
	return p * common_factor;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 2);
	REQUIRE(solve_part1(path(dataDir) / path("inputExample2.txt")) == 6);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample3.txt")) == 6);
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
