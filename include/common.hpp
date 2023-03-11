#ifndef BASH_CLI_COMMON_HPP
#define BASH_CLI_COMMON_HPP

#include <boost/process/environment.hpp>

#include <string>
#include <vector>
#include <cstdio>
#include <optional>

struct token;
using Environment = boost::process::basic_native_environment<char>;
using PipeLine = std::vector<token>;

namespace FileUtils {

inline int writeToFile(const std::string& str, FILE* file) {
    if (str.size() != fwrite(str.c_str(), sizeof(char), str.size(), file))
        return 1;
    return 0;
}

inline std::optional<std::string> readLine(FILE* file) {
    char* line = nullptr;
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

} // namespace FileUtils

struct token {
    std::string name;
    std::vector<std::string> args;

    /**
     * Add a word to token: if name empty assigne this word to name,
     *                    else add word to args.
     *
     * @param `word` strings - some word: command name or argument.
     */
    void add_word(const std::string & word) {
        if (name.empty()) {
          name = word;
        } else {
          args.push_back(word);
        }
    }
};

#endif //BASH_CLI_COMMON_HPP
