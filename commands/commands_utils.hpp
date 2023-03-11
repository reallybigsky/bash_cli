#ifndef BASH_CLI_COMMANDS_UTILS_HPP
#define BASH_CLI_COMMANDS_UTILS_HPP

#include "cmd.hpp"
#include "common.hpp"

#include <filesystem>
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <fstream>


namespace commands {

namespace fs = std::filesystem;
namespace bp = boost::process;

inline bool is_file_exist(const fs::path &full_filepath) {
    return fs::exists(full_filepath);
}

inline bool is_readable(const fs::path &filepath) {
    return std::ifstream(filepath).is_open();
}

inline bool is_writable(const fs::path &filepath) {
    return std::ofstream(filepath).is_open();
}

}

#endif // BASH_CLI_COMMANDS_UTILS_HPP