#pragma once

#include "file_utils.hpp"

#include <gtest/gtest.h>

#include <cstdio>


std::string read_file_to_string(FileStream& out){
    out.seek_begin();
    std::stringstream result;
    while (auto line = out.read_line())
        result << line.value();

    out.seek_begin();
    return result.str();
}

void create_testfile(const std::string& filename, const std::vector<std::string>& file_content) {
    std::fstream f1(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
    if (f1.is_open()) {
        for (const auto& s : file_content) {
            f1 << s << std::endl;
        }
    } else {
        std::cout << filename << "NOT OPENED" << std::endl;
    }
}
