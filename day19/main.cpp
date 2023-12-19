// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <filesystem>
#include <iostream>
#include <regex>
#include <stdint.h>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

using rule_t = tuple<char, char, int, string>; // (variable, <>!, number, target)
using workflow_t = vector<rule_t>;
using workflows_t = unordered_map<string, workflow_t>;
using ratings_t = tuple<int, int, int, int>; // (x, m, a, s)

pair<string, workflow_t> parseLine(const string& line)
{
	regex re(R"((\w+)\{([a-zA-Z0-9:,<>]+)\})");
	smatch m;
	regex_match(line, m, re);
	auto rule_name = m[1].str();
	auto rules_str = m[2].str();

	auto tokens = split(regex(","), rules_str);
	regex re2(R"((\w+)(([><])(\d+):(\w+))?)");
	vector<rule_t> rules;
	for(auto token : tokens)
	{
		smatch m2;
		regex_match(token, m2, re2);

		string target;
		if(m2[2].matched)
		{
			auto s1 = m2[1].str()[0];
			char c = '!';
			int v = 0;
			c = m2[3].str()[0];
			v = stoi(m2[4].str());
			target = m2[5].str();
			rules.push_back(rule_t{s1, c, v, target});
		} else
		{
			target = m2[1].str();
			rules.push_back(rule_t{'_', '!', 0, target});
		}
	}

	return {rule_name, rules};
}

ratings_t parseLine2(const string& line)
{
	auto ns = extractIntegers(line);
	return ratings_t((ns[0]), (ns[1]), (ns[2]), (ns[3]));
}

auto parseLines(const vector<string>& lines)
{
	workflows_t ws;
	vector<ratings_t> rs;
	int part = 0;
	for(auto& line : lines)
	{
		if(line.empty())
		{
			part++;
			continue;
		}
		if(part == 0)
		{
			auto [n, w] = parseLine(line);
			ws[n] = w;
		} else
		{
			rs.push_back(parseLine2(line));
		}
	}
	return make_tuple(ws, rs);
}

auto do_rate(ratings_t rating, workflows_t& ws)
{
	string curr = "in";
	while(curr != "R" && curr != "A")
	{
		auto& rules = ws.at(curr);
		for(auto& [c, cmp, v, target] : rules)
		{
			if(cmp == '!')
			{
				curr = target;
				break;
			} else
			{
				auto& [x, m, a, s] = rating;
				int vv;
				switch(c)
				{
					case 'x':
						vv = x;
						break;
					case 'm':
						vv = m;
						break;
					case 'a':
						vv = a;
						break;
					case 's':
						vv = s;
						break;
				}
				if(cmp == '<')
				{
					if(vv < v)
					{
						curr = target;
						break;
					}
				} else
				{
					if(vv > v)
					{
						curr = target;
						break;
					}
				}
			}
		}
	}
	return curr == "A";
}

auto solve_part1(const path& inputFile)
{
	auto [ws, rs] = parseLines(readLines(inputFile));
	int sum{};
	for(auto r : rs)
	{
		bool accepted = do_rate(r, ws);
		if(accepted)
		{
			auto [x, m, a, s] = r;
			sum += x + m + a + s;
		}
	}

	return sum;
}

using box_t = pair<ratings_t, ratings_t>; // min, max (both is inclusive)

void do_rate2(string curr, box_t rating_box, const workflows_t& ws, vector<box_t>& accept_boxes)
{
	if(curr == "R")
		return;
	if(curr == "A")
	{
		accept_boxes.push_back(rating_box);
		return;
	}

	auto& rules = ws.at(curr);
	for(auto& [c, cmp, v, target] : rules)
	{
		if(cmp == '!')
		{
			do_rate2(target, rating_box, ws, accept_boxes);
			return;
		} else
		{
			auto [x1, m1, a1, s1] = get<0>(rating_box);
			auto [x2, m2, a2, s2] = get<1>(rating_box);

			box_t box1;
			box_t box2;
			if(cmp == '<')
			{
				switch(c)
				{
					case 'x':
						box1 = box_t{{x1, m1, a1, s1}, {v - 1, m2, a2, s2}};
						box2 = box_t{{v, m1, a1, s1}, {x2, m2, a2, s2}};
						break;
					case 'm':
						box1 = box_t{{x1, m1, a1, s1}, {x2, v - 1, a2, s2}};
						box2 = box_t{{x1, v, a1, s1}, {x2, m2, a2, s2}};
						break;
					case 'a':
						box1 = box_t{{x1, m1, a1, s1}, {x2, m2, v - 1, s2}};
						box2 = box_t{{x1, m1, v, s1}, {x2, m2, a2, s2}};
						break;
					case 's':
						box1 = box_t{{x1, m1, a1, s1}, {x2, m2, a2, v - 1}};
						box2 = box_t{{x1, m1, a1, v}, {x2, m2, a2, s2}};
						break;
				}
				do_rate2(target, box1, ws, accept_boxes);
				rating_box = box2;
			} else
			{ // >
				switch(c)
				{
					case 'x':
						box1 = box_t{{x1, m1, a1, s1}, {v, m2, a2, s2}};
						box2 = box_t{{v + 1, m1, a1, s1}, {x2, m2, a2, s2}};
						break;
					case 'm':
						box1 = box_t{{x1, m1, a1, s1}, {x2, v, a2, s2}};
						box2 = box_t{{x1, v + 1, a1, s1}, {x2, m2, a2, s2}};
						break;
					case 'a':
						box1 = box_t{{x1, m1, a1, s1}, {x2, m2, v, s2}};
						box2 = box_t{{x1, m1, v + 1, s1}, {x2, m2, a2, s2}};
						break;
					case 's':
						box1 = box_t{{x1, m1, a1, s1}, {x2, m2, a2, v}};
						box2 = box_t{{x1, m1, a1, v + 1}, {x2, m2, a2, s2}};
						break;
				}
				do_rate2(target, box2, ws, accept_boxes);
				rating_box = box1;
			}
		}
	}
	std::unreachable();
}

auto solve_part2(const path& inputFile)
{
	auto [ws, rs] = parseLines(readLines(inputFile));

	box_t start{{1, 1, 1, 1}, {4000, 4000, 4000, 4000}};
	vector<box_t> accept_boxes;
	do_rate2("in", start, ws, accept_boxes);

	int64_t sum{};
	for(auto [b1, b2] : accept_boxes)
	{
		auto [x1, m1, a1, s1] = b1;
		auto [x2, m2, a2, s2] = b2;
		sum += int64_t(x2 + 1 - x1) * int64_t(m2 + 1 - m1) * int64_t(a2 + 1 - a1) *
		       int64_t(s2 + 1 - s1);
	}
	return sum;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 19114);
}

TEST_CASE("examples-part2", "[solve_part2]")
{
	REQUIRE(solve_part2(path(dataDir) / path("inputExample1.txt")) == 167409079868000);
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
