#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace tools {

    // Строковые утилиты
    std::vector<std::string> split(const std::string& s, char delim = ' ');
    std::string join(const std::vector<std::string>& parts, const std::string& sep = " ");

    // JSON утилиты
    nlohmann::json parse_json(const std::string& raw);
    nlohmann::json user_command_to_json(const std::string& line);
    std::string get_cmd(const nlohmann::json& j);
    std::string get_arg(const nlohmann::json& j, const std::string& key);

    // Формирование ответов
    nlohmann::json ok(const std::string& data = "");
    nlohmann::json error(const std::string& err);
    bool is_error(const nlohmann::json& j);
    std::string get_error(const nlohmann::json& j);
    std::string get_data(const nlohmann::json& j);

}

#endif
