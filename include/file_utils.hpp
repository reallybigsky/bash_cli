#ifndef BASH_CLI_FILE_UTILS_HPP
#define BASH_CLI_FILE_UTILS_HPP

#include "common.hpp"

#include <filesystem>
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <fstream>

#include <boost/process.hpp>

#ifdef _WIN32
#include "getline_win.hpp"
#endif

namespace FileUtils {

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

inline int writeToFile(const std::string &str, FILE *file) {
    if (str.size() != fwrite(str.c_str(), sizeof(char), str.size(), file))
        return 1;
    return 0;
}

inline std::optional<std::string> readLine(FILE *file) {
    char *line = nullptr;
    size_t cnt = 0;
    size_t read = getline(&line, &cnt, file);
    std::optional<std::string> res;
    if (read == -1) {
        res = std::nullopt;
    } else {
        res = std::string(line, read);
    }
    free(line);
    return res;

} //namespace FIleUtils

}

#endif //BASH_CLI_FILE_UTILS_HPP
