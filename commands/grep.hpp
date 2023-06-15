#pragma once

#include "cmd.hpp"
#include "file_utils.hpp"

#include <boost/program_options.hpp>
#include <boost/regex.hpp>

namespace Commands {

namespace po = boost::program_options;

/**
    * Implementation of grep command
*/
class Grep : public Cmd {

    po::options_description desc = po::options_description("Usage: grep [OPTION]... PATTERNS [FILE]...\n"
                                                           "Search for PATTERNS in each FILE.\n"
                                                           "Example: grep -i 'hello world' menu.h main.c\n"
                                                           "PATTERNS can contain multiple patterns separated by newlines.\n"
                                                           "\n"
                                                           "Pattern selection and interpretation");
    po::positional_options_description p;
public:
    Grep() {
        desc.add_options()
                ("help", "display this help text and exit")
                ("regexp,e", po::value<std::string>(), "use PATTERNS for matching")
                ("file,f", po::value<std::vector<std::string>>(), "take PATTERNS from FILE")
                ("ignore-case,i", "ignore case distinctions in patterns and data")
                ("word-regexp,w", "match only whole words")
                ("after-context,A", po::value<int>(), "print NUM lines of output context");

        p.add("regexp", 1);
        p.add("file", -1);
    }

    /**
    * Searches for PATTERNS in each FILE.
    * PATTERNS is one or more patterns separated by newline characters, and grep prints each line that matches a pattern.
    * If there are no files, then the search occurs in the lines supplied from the passed input stream
    *
    * @param params: CmdToken with command name in params.name and command arguments in params.args
    * @param env: current environment variables of the interpreter
    * @param input: input FileStream
    * @param output: output FileStream
    * @param err: error FileStream
    * @return 0 if there were no errors, 1 otherwise
    */
    virtual int run(const CmdToken& params, std::shared_ptr<Environment> env, FileStream& input, FileStream& output, FileStream& err) const override {
        std::vector<const char*> args_with_options;
        args_with_options.push_back(params.name.c_str());
        for (auto& arg: params.args) {
            args_with_options.push_back(arg.c_str());
        }

        po::variables_map vm;
        po::store(po::command_line_parser(static_cast<int>(args_with_options.size()), args_with_options.data()).
                options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::stringstream result;
            result << desc << "\n";
            output << result.str();
            return 0;
        }

        if (!vm.count("regexp")) {
            err << "Usage: grep [OPTION]... PATTERNS [FILE]...\n"
                   "Try 'grep --help' for more information.";
            return 1;
        }


        boost::regex_constants::syntax_option_type flags = vm.contains("ignore-case")
                                                           ? boost::regex_constants::icase
                                                           : boost::regex_constants::normal;
        int after_context_NUM = vm.contains("after-context") ? vm["after-context"].as<int>() : 0;
        std::string pattern = vm.contains("word-regexp") ? "[[:<:]]" + vm["regexp"].as<std::string>() + "[[:>:]]"
                                                         : vm["regexp"].as<std::string>();

        if(after_context_NUM < 0) {
            err << params.name << ": " << std::to_string(after_context_NUM) << ": invalid context length argument";
            return 1;
        }

        boost::regex base_regex(pattern, flags);

        if (!vm.count("file")) {
            boost::smatch base_match;
            while (auto line = input.read_line()) {
                if (boost::regex_search(line.value(), base_match, base_regex)) {
                    output << line.value();
                }
            }
            return 0;
        }


        auto& files = vm["file"].as<std::vector<std::string>>();
        bool greater_one = files.size() > 1;

        std::stringstream result, errors;
        size_t error_counter = 0;
        for (auto& filename: files) {
            auto result_validation = file_validation_check(result, params.name, env->current_path, filename, error_counter);
            if (!result_validation.error_message.empty())
                continue;

            result << matching_in_file(filename, result_validation.full_filepath, base_regex, after_context_NUM, greater_one);
        }

        if (error_counter == files.size()) {
            err << result.str();
            return 1;
        }

        output << result.str();

        if (error_counter > 0)
            return 1;

        return 0;
    }


private:
    std::string matching_in_file(
            const std::string& original_name,
            std::filesystem::path& filename,
            boost::regex& base_regex,
            int after_context_NUM,
            bool greater_one) const {

        std::fstream file(filename);

        std::stringstream result;
        std::string line;
        boost::smatch base_match;
        bool first = true;
        while (std::getline(file, line)) {
            if (!boost::regex_search(line, base_match, base_regex))
                continue;

            if (first) {
                first = false;
            } else if (after_context_NUM > 0) {
                result << "--" << std::endl;
            }

            if (greater_one)
                result << original_name << ":";
            result << line << std::endl;

            for (size_t i = 0; i < after_context_NUM && std::getline(file, line); ++i) {
                if (boost::regex_search(line, base_match, base_regex)) {
                    if (greater_one)
                        result << original_name << ":";
                    i = 0;
                } else if (greater_one)
                    result << original_name << "-";

                result << line << std::endl;
            }

        }

        return result.str();
    }
};

} //commands
