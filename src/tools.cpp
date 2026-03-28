#include "tools.hpp"
#include <sstream>
#include <algorithm>

namespace tools {

    // ========== СТРОКОВЫЕ УТИЛИТЫ ==========

    std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string token;
        while (std::getline(ss, token, delim)) {
            if (!token.empty()) {
                result.push_back(token);
            }
        }
        return result;
    }

    std::string join(const std::vector<std::string>& parts, const std::string& sep) {
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i != 0) result += sep;
            result += parts[i];
        }
        return result;
    }

    // ========== JSON УТИЛИТЫ ==========

    nlohmann::json parse_json(const std::string& raw) {
        try {
            auto j = nlohmann::json::parse(raw);
            if (!j.is_object()) {
                return nlohmann::json::object();
            }
            return j;
        } catch (...) {
            return nlohmann::json::object();
        }
    }

    nlohmann::json user_command_to_json(const std::string& line) {

        if (line.rfind("BND ", 0) == 0) {
            return {{"cmd", "BND"}, {"target", line.substr(4)}};
        } else if (line.rfind("MSG ", 0) == 0) {
            return {{"cmd", "MSG"}, {"text", line.substr(4)}};
        } else if (line.rfind("REG ", 0) == 0) {
            return {{"cmd", "REG"}, {"name", line.substr(4)}};
        } else if (line == "EXT") {
            return {{"cmd", "EXT"}};
        }
        return {{"cmd", "UNK"}, {"text", line}};
    }

    std::string get_cmd(const nlohmann::json& j) {
        if (!j.is_object()) return "";
        return j.value("cmd", "");
    }

    std::string get_arg(const nlohmann::json& j, const std::string& key) {
        if (!j.is_object()) return "";
        auto it = j.find(key);
        if (it == j.end()) return "";
        if (!it->is_string()) return "";
        return it->get<std::string>();
    }

    // ========== ФОРМИРОВАНИЕ ОТВЕТОВ ==========

    nlohmann::json ok(const std::string& data) {
        nlohmann::json j = {{"status", "ok"}};
        if (!data.empty()) {
            j["data"] = data;
        }
        return j;
    }

    nlohmann::json error(const std::string& err) {
        return {{"status", "error"}, {"error", err}};
    }

    bool is_error(const nlohmann::json& j) {
        return j.value("status", "") == "error";
    }

    std::string get_error(const nlohmann::json& j) {
        return j.value("error", "Unknown error");
    }

    std::string get_data(const nlohmann::json& j) {
        return j.value("data", "");
    }

}
