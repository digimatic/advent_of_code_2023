// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>
#include <common/tuple_hash.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <limits>
#include <stdint.h>
#include <string_view>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

auto solve_part1(const path& inputFile)
{
	auto lines = readLines(inputFile);
	auto seeds = extractIntegers<int64_t>(lines[0]);
	vector<vector<vector<int64_t>>> maps;
	for(int i = 2; i < lines.size();)
	{
		auto& line = lines[i];
		++i; // skip label

		// dest start, sourcte start, length
		vector<vector<int64_t>> numbers;
		while(i < lines.size() && lines[i].length() != 0)
		{
			auto ns = extractIntegers<int64_t>(lines[i]);
			numbers.push_back(ns);
			++i;
		}
		maps.push_back(numbers);
		++i; // skip empty line
	}

	int64_t min_dest = numeric_limits<int64_t>::max();
	for(int64_t c : seeds)
	{
		for(auto& map : maps)
		{
			for(auto& r : map)
			{
				auto dest_start = r[0];
				auto src_start = r[1];
				auto count = r[2];

				if(c >= src_start && c < (src_start + count))
				{
					c += dest_start - src_start;
					break;
				}
			}
		}
		min_dest = min(min_dest, c);
	}

	return min_dest;
}

auto solve_part2(const path& inputFile)
{
	auto lines = readLines(inputFile);
	auto ss = extractIntegers<int64_t>(lines[0]);
	vector<pair<int64_t, int64_t>> seeds;
	for(int i = 0; i < ss.size(); i += 2)
	{
		seeds.emplace_back(ss[i], ss[i + 1]);
	}

	vector<vector<vector<int64_t>>> maps;
	for(int i = 2; i < lines.size();)
	{
		auto& line = lines[i];
		auto name = line;
		++i; // skip label

		// dest start, sourcte start, length
		vector<vector<int64_t>> numbers;
		while(i < lines.size() && lines[i].length() != 0)
		{
			auto ns = extractIntegers<int64_t>(lines[i]);
			numbers.push_back(ns);
			++i;
		}
		maps.push_back(numbers);
		++i; // skip empty line
	}

	int64_t min_dest = numeric_limits<int64_t>::max();
	for(auto [seed_start0, seed_count0] : seeds)
	{
		cout << "Process seed: [" << seed_start0 << ".." << (seed_start0 + seed_count0) << "]\n";
		unordered_set<tuple<int64_t, int64_t>> s;
		s.emplace(seed_start0, seed_count0);
		int mapi = 0;
		for(auto& mappings : maps)
		{
			mapi++;
			vector<pair<int64_t, int64_t>> s2;
			// split source ranges from s to s2 so they are fittet inside the mapping ranges.
			while(!s.empty())
			{
				bool found = false;
				const auto [seed_start, seed_count] = *(s.begin());
				const auto seed_end = seed_start + seed_count;
				s.erase(s.begin());
				if(seed_count == 0)
					continue;
				if(seed_count < 0)
					throw -4;

				for(auto& r : mappings)
				{
					auto src_start = r[1];
					auto count = r[2];
					auto src_end = src_start + count;

					if(seed_start < src_start && seed_end > src_end)
					{
						// seed range fully encloses the range. split in 3 ranges.
						s.emplace(seed_start, src_start - seed_start);
						s2.emplace_back(src_start, src_end - src_start);
						s.emplace(src_end, seed_end - src_end);
						found = true;
						break;
					} else if(seed_end <= src_start || seed_start >= src_end)
					{
						// seedrange outside source range. try next range.
					} else if(seed_start >= src_start && seed_end <= src_end)
					{
						// totaly inside
						if(seed_end != seed_start)
							s2.emplace_back(seed_start, seed_end - seed_start);
						found = true;
						break;
					} else if(seed_start < src_start)
					{
						// crossing src start
						s.emplace(seed_start, src_start - seed_start);
						s2.emplace_back(src_start, seed_end - src_start);
						found = true;
						break;
					} else
					{
						// crossing src_end
						s2.emplace_back(seed_start, src_end - seed_start);
						s.emplace(src_end, seed_end - src_end);
						found = true;
						break;
					}
				}
				if(!found)
				{
					s2.emplace_back(seed_start, seed_count);
				}
			}

			for(auto [seed_start, seed_count] : s2)
			{
				if(seed_count == 0)
					continue;
				auto seed_end = seed_start + seed_count;
				for(auto& r : mappings)
				{
					auto dest_start = r[0];
					auto src_start = r[1];
					auto count = r[2];
					auto dest_end = dest_start + count;
					auto src_end = src_start + count;

					if(seed_start >= src_start && seed_end <= src_end)
					{
						seed_start += dest_start - src_start;
						break;
					}
				}
				s.emplace(seed_start, seed_count);
			}
		}

		for(auto [c, _] : s)
		{
			min_dest = min(c, min_dest);
		}
	}

	return min_dest;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 35);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 46);
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
