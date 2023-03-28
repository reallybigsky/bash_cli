#ifndef BASH_CLI_TEST_UTILS_HPP
#define BASH_CLI_TEST_UTILS_HPP
#include <gtest/gtest.h>
#include "file_utils.hpp"
#include <stdio.h>


std::string read_file_to_string(FILE* out){
    rewind(out);
    std::stringstream result;
    while (auto line = FileUtils::readLine(out))
        result << line.value();

    rewind(out);

    return result.str();
}

void create_testfile(const std::string& filename, const std::vector<std::string>& file_content) {
    std::fstream f1(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
    if (f1.is_open()) {
        for (const auto& s : file_content) {
            f1 << s << std::endl;
        }
        f1.close();
    } else
        std::cout << filename << "NOT OPENED" << std::endl;
}

#endif //BASH_CLI_TEST_UTILS_HPP
