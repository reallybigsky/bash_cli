#ifndef BASH_CLI_COMMON_HPP
#define BASH_CLI_COMMON_HPP

#include <boost/process/environment.hpp>

#include <string>
#include <vector>

//TODO: documentation

using Environment = boost::process::basic_native_environment<char>;


inline int writeToFile(const std::string& str, FILE* file) {
    if (str.size() != fwrite(str.c_str(), sizeof(char), str.size(), file))
        return 1;
    return 0;
}

struct job {
    std::string name;
    std::vector<std::string> args;

    /**
     * Add a word to job: if name empty assigne this word to name,
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
