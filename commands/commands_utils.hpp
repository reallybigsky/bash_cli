#pragma once
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <fstream>
#include "common.hpp"


namespace commands {

    namespace fs = std::filesystem;
    namespace bp = boost::process;

    bool is_file_exist(const fs::path &full_filepath) {
        return fs::exists(full_filepath);
    }

    bool is_readable(const fs::path &filepath) {
        return std::ifstream(filepath).is_open();
    }

    bool is_writable(const fs::path &filepath) {
        return std::ofstream(filepath).is_open();
    }

}
