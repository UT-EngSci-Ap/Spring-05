#include "ConfigManager.h"

#include <fstream>
#include <iostream>
#include <utility>

namespace {

std::string trim(const std::string& s) {
    const char* ws = " \t\r\n";
    std::size_t begin = s.find_first_not_of(ws);
    if (begin == std::string::npos) {
        return "";
    }
    std::size_t end = s.find_last_not_of(ws);
    return s.substr(begin, end - begin + 1);
}

}  // namespace

ConfigManager::ConfigManager(std::string path) : path_(std::move(path)) {}

void ConfigManager::load() {
    std::ifstream in(path_);
    if (!in.is_open()) {
        return;   // no settings yet; defaults will be used
    }
    std::string line;
    while (std::getline(in, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') {
            continue;
        }
        std::size_t eq = trimmed.find('=');
        if (eq == std::string::npos) {
            continue;
        }
        std::string key = trim(trimmed.substr(0, eq));
        std::string value = trim(trimmed.substr(eq + 1));
        if (!key.empty()) {
            values_[key] = value;
        }
    }
}

void ConfigManager::save() const {
    std::ofstream out(path_);
    if (!out.is_open()) {
        std::cerr << "[ConfigManager] Could not write " << path_ << '\n';
        return;
    }
    for (const auto& [key, value] : values_) {
        out << key << '=' << value << '\n';
    }
}

std::string ConfigManager::get(const std::string& key,
                               const std::string& fallback) const {
    auto it = values_.find(key);
    return it != values_.end() ? it->second : fallback;
}

void ConfigManager::set(const std::string& key, const std::string& value) {
    values_[key] = value;
}
