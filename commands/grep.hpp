#ifndef BASH_CLI_GREP_HPP
#define BASH_CLI_GREP_HPP
#include "cmd.hpp"
#include "file_utils.hpp"
#include <boost/program_options.hpp>

namespace commands {
/**
 * Implementation of echo command
 */

    namespace po = boost::program_options;

    class Grep : public Cmd {
        virtual int run(const token& tok, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE* err) {
            std::vector<const char*> args_with_options;
            args_with_options.push_back(tok.name.c_str());
            for(auto& arg: tok.args) {
                args_with_options.push_back(arg.c_str());
            }

            po::options_description desc("Usage: grep [OPTION]... PATTERNS [FILE]...\n"
                                         "Search for PATTERNS in each FILE.\n"
                                         "Example: grep -i 'hello world' menu.h main.c\n"
                                         "PATTERNS can contain multiple patterns separated by newlines.\n"
                                         "\n"
                                         "Pattern selection and interpretation:");
            desc.add_options()
                    ("help","display this help text and exit")
                    ("ignore-case,i", "ignore case distinctions in patterns and data")
                    ("no-ignore-case", "do not ignore case distinctions (default)")
                    ("after-context,A", po::value<int>(),  "print NUM lines of output context")
                    ("word-regexp,w", "match only whole words")
                    ;

            po::variables_map vm;
            po::store(po::parse_command_line(args_with_options.size(), args_with_options.data(), desc), vm);
            po::notify(vm);

            std::stringstream result;

            if(vm.count("help"))
            {
                result << desc << "\n";
                FileUtils::writeToFile(result.str(), output);
            }

            return 0;
        }
    };
}
#endif //BASH_CLI_GREP_HPP
