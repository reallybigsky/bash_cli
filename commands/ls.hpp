#ifndef BASH_CLI_LS_HPP
#define BASH_CLI_LS_HPP


#include <filesystem>
#include "cmd.hpp"
#include "file_utils.hpp"

namespace commands {

    /**
     * Implementation of ls command
     */
    class Ls : public Cmd {

    public:

        /**
         * Prints all files and directories from the passed as the param (not recursively)
         * @param params If zero parameter - prints content of current directory.
         * Otherwise - content of "current_directory/parameter" for each parameter
         * @param env: current environment variables of the interpreter
         * @param input: input FILE stream
         * @param output: output FILE stream
         * @param err: error FILE stream
         * @return
         */
        virtual int run(const token &params, std::shared_ptr<Environment> env, FILE *input, FILE *output, FILE *err) {
            std::filesystem::path current_path(env->at("PWD").to_string());

            // принтим просто список файлов текущей директории
            if (params.args.empty()) {
                current_path += "/";
                FileUtils::writeToFile(directory_iterator(current_path), output);
                return 0;
            }

            // иначе - принтим список файлов в директории "текущая/то_что_передали"
            std::stringstream result;
            std::stringstream errors;
            int32_t error_count = 0;
            for (auto &directory: params.args) {
                std::filesystem::path path = current_path;
                path += "/" + directory;
                if (!FileUtils::is_directory_exist(path)) {
                    ++error_count;
                    result << params.name << ": " << directory << ": No such directory" << std::endl;
                    errors << params.name << ": " << directory << ": No such directory" << std::endl;
                    continue;
                }
                if (params.args.size() > 1) {
                    result << directory << ":" << std::endl;
                }
                result << directory_iterator(directory) << std::endl;
            }

            if (error_count == params.args.size()) {
                FileUtils::writeToFile(errors.str(), err);
                return 1;
            }

            FileUtils::writeToFile(result.str(), output);
            return 0;
        }

    private:

        /**
         * Iterates throw the directory and constructs the result of all files and directories
         * @param path to the directory to iterate
         * @return string with all files and directories (not recursively)
         */
        static std::string directory_iterator(const std::filesystem::path &path) {
            std::stringstream result;
            for (auto const &dir_entry: std::filesystem::directory_iterator(path)) {
                std::string entry = dir_entry.path().string();
                entry = entry.substr(entry.find_last_of('\\') + 1);
                if (FileUtils::is_directory_exist(dir_entry.path().string())) {
                    entry += "/";
                }
                result << entry << std::endl;
            }
            return result.str();
        }
    };
} // namespace commands

#endif //BASH_CLI_LS_HPP