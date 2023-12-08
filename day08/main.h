#pragma once
int run_process(std::string& current, std::string& target, std::string& instructions,
                std::unordered_map<std::string, std::pair<std::string, std::string>>& map,
                int& sum);
