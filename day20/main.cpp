// Advent of Code 2023
// Peter Westerström (digimatic)

#include "config.h"

#include <common/common.h>

#include <filesystem>
#include <iostream>
#include <queue>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace westerstrom;
using namespace std;
using std::filesystem::path;

using module_t = tuple<char, vector<string>>;
using modules_t = unordered_map<string, module_t>;
using pulse_t = tuple<string, bool, string>;
using states_t = unordered_map<string, bool>;
using inputs_t = unordered_map<string, states_t>;

auto parseLine(const string& line)
{
	char type;
	string line2;
	if(line.starts_with("broadcaster"))
	{
		type = 'B';
		line2 = line;
	} else
	{
		type = line[0];
		line2 = line.substr(1);
	}

	auto tokens = split(regex(R"(,?\s(->\s)?)"), line2);
	auto name = tokens[0];
	tokens.erase(tokens.begin());
	return make_tuple(type, name, tokens);
}

auto parseLines(const vector<string>& lines)
{
	modules_t modules;
	int i = 0;
	int b = -1;
	for(auto& line : lines)
	{
		auto [t, n, d] = parseLine(line);
		modules[n] = make_tuple(t, d);
		i++;
	}
	return modules;
}

void send_pulse(modules_t& modules, states_t& states, inputs_t& inputs, const string& module,
                const string& source, const bool signal, queue<pulse_t>& q)
{
	auto [type, targets] = modules[module];
	if(type == 'B')
	{
		for(auto& target : targets)
		{
			q.emplace(target, signal, module);
		}
	} else if(type == '%')
	{
		if(!signal)
		{
			bool s = !states[module];
			states[module] = s;
			for(auto& target : targets)
			{
				q.emplace(target, s, module);
			}
		}
	} else if(type == '&')
	{
		auto& input_states = inputs[module];
		input_states[source] = signal;
		bool bb = true;
		for(auto& [inp, state] : input_states)
		{
			bb = bb && state;
		}
		bb = !bb;
		for(auto& target : targets)
		{
			q.emplace(target, bb, module);
		}
	} else
	{
		std::unreachable();
	}
}

void find_module_inputs(const modules_t& modules, inputs_t& inputs0, states_t& states)
{
	for(auto& [s, m] : modules)
	{
		auto& [type, targets] = m;
		for(auto target : targets)
		{
			inputs0[target][s] = false;
		}
		states[s] = false;
	}
}

auto solve_part1(const path& inputFile)
{
	auto modules = parseLines(readLines(inputFile));
	states_t states;
	inputs_t inputs;
	find_module_inputs(modules, inputs, states);
	int low_pulses_sent = 0;
	int high_pulses_sent = 0;
	for(int button_i = 0; button_i < 1000; button_i++)
	{
		queue<pulse_t> q;
		q.push(pulse_t{"broadcaster", false, "button"});

		while(!q.empty())
		{
			auto [module, signal, source] = q.front();
			q.pop();
			if(signal)
			{
				high_pulses_sent++;
			} else
			{
				low_pulses_sent++;
			}
			send_pulse(modules, states, inputs, module, source, signal, q);
		}
	}
	return high_pulses_sent * low_pulses_sent;
}

// Input data analysis
//
// &qs -> gf
// &sv -> gf
// &pg -> gf
// &sp -> gf
//
// &gf -> rx
auto solve_part2(const path& inputFile)
{
	auto modules = parseLines(readLines(inputFile));
	states_t states;
	inputs_t inputs;
	find_module_inputs(modules, inputs, states);

	long long button_i{};
	string found = "";
	auto rxInput = get<0>(*(inputs["rx"].begin())); // gf
	unordered_set<string> rxInputInputs;            // -> qs, sv, pg, sp
	for(auto [rxInpInp, v] : inputs[rxInput])
	{
		rxInputInputs.insert(rxInpInp);
	}
	vector<long long> pushes;

	while(true)
	{
		button_i++;

		queue<pulse_t> q;
		q.push(pulse_t{"broadcaster", false, "button"});
		while(!q.empty())
		{
			auto [module, signal, source] = q.front();
			q.pop();

			if(rxInputInputs.contains(source) && module == rxInput && signal)
			{
				found = source;
				rxInputInputs.erase(source);
			}

			send_pulse(modules, states, inputs, module, source, signal, q);
		}

		if(!found.empty())
		{
			pushes.push_back(button_i);
			if(rxInputInputs.size() == 0)
				break;
			found = "";
		}
	}

	// assumes they are prime numbers
	auto r = pushes[0] * pushes[1] * pushes[2] * pushes[3];
	return r;
}

TEST_CASE("examples-part1", "[solve_part1]")
{
	REQUIRE(solve_part1(path(dataDir) / path("inputExample1.txt")) == 32000000);
	REQUIRE(solve_part1(path(dataDir) / path("inputExample2.txt")) == 11687500);
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
