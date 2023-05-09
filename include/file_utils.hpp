#pragma once

#include "common.hpp"

#include <filesystem>
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <fstream>


#if defined(_WIN64) || defined(_WIN32)
#include "getline_win.hpp"
#endif

namespace FileUtils {

namespace fs = std::filesystem;
//namespace bp = boost::process;

/**
 * Check if file exists in given path
 *
 * @param full_filepath
 * @return true if file exists and false otherwise
 */
inline bool is_file_exist(const fs::path &full_filepath) {
    return fs::exists(full_filepath);
}

/**
 * Check if file is readable
 *
 * @param filepath
 * @return true if file is readable and false otherwise
 */
inline bool is_readable(const fs::path &filepath) {
    return std::ifstream(filepath).is_open();
}

/**
 * Check if file is writable
 *
 * @param filepath
 * @return true if file is readable and false otherwise
 */
inline bool is_writable(const fs::path &filepath) {
    return std::ofstream(filepath).is_open();
}

/**
 * Write string to FILE stream
 *
 * @param str: string to write
 * @param file: FILE stream where str will be written
 * @return 1 there was an error in writing, else 0
 */
inline int writeToFile(const std::string &str, FILE *file) {
    if (str.size() != fwrite(str.c_str(), sizeof(char), str.size(), file))
        return 1;
    return 0;
}

/**
 * Read one line from file
 *
 * @param file: FILE stream to read from
 * @return string line from file if EOF or errors weren't encountered, std::nullopt otherwise
 */
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
}

} //namespace FIleUtils
