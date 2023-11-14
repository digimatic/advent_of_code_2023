#pragma once

#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

namespace westerstrom
{
	inline std::vector<std::string> readLines(const std::filesystem::path& inputFile)
	{
		std::vector<std::string> lines;
		std::fstream f(inputFile);

		while(!f.eof())
		{
			std::string line;
			if(std::getline(f, line))
			{
				line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
				line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
				lines.push_back(line);
			}
		}
		return lines;
	}

	inline std::vector<std::string> readLines(const std::string& inputFile)
	{
		return readLines(std::filesystem::path(inputFile));
	}

	inline std::vector<std::string> split(const std::regex& re, const std::string& subject)
	{
		using namespace std;
		vector<string> container{sregex_token_iterator(subject.begin(), subject.end(), re, -1),
		                         sregex_token_iterator()};
		return container;
	}

	/*inline std::vector<int> splitNumbers(const std::string& line, bool signedNumbers = false)
	{
		using namespace std;
		vector<int> ints;
		regex re(signedNumbers ? "[^-\\d]+" : "\\D+");
		transform(sregex_token_iterator(line.begin(), line.end(), re, -1), sregex_token_iterator(),
		          back_inserter(ints), [](const auto& s) -> int {
			          try
			          {
				          return stoi(s);
			          } catch(const invalid_argument&)
			          {
				          return 0;
			          }
		          });
		return ints;
	}*/

	template <class T=int>
	std::vector<T> extractIntegers(const std::string& str, bool signedNumbers = false)
	{
		std::vector<T> integers;
		std::regex integer_regex(signedNumbers ? "[-+]?\\d+" : "\\d+");
		std::smatch match;

		std::string::const_iterator searchStart(str.cbegin());
		while(std::regex_search(searchStart, str.cend(), match, integer_regex))
		{
			T val{};
			std::from_chars(&(*(match[0].first)), &(*(match[0].second)), val);
			integers.push_back(val);
			searchStart = match.suffix().first;
		}

		return integers;
	}

	inline int findEmptyLineIndex(const std::vector<std::string>& lines)
	{
		auto it = std::find(std::begin(lines), std::end(lines), std::string());
		if(it != std::end(lines))
		{
			return std::distance(std::begin(lines), it);
		}
		return -1;
	}

} // namespace westerstrom
