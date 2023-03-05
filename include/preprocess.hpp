#ifndef BASH_CLI_PREPROCESS_HPP
#define BASH_CLI_PREPROCESS_HPP

#include "common.hpp"

#include <vector>
#include <string>

namespace preprocess {

std::vector<std::string> runLexer(const std::string& input) {
    return {};
}

std::vector<job> runParser(const std::vector<std::string>& string_commands) {
    return {};
}


} // namespace preprocess

#endif //BASH_CLI_PREPROCESS_HPP
