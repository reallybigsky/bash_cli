#ifndef BASH_CLI_EXTCMD_HPP
#define BASH_CLI_EXTCMD_HPP

#include "file_utils.hpp"

#include <boost/process.hpp>

#include <numeric>

namespace commands {

namespace bp = boost::process;

/**
 * Implementation of external command (not internal command)
 */
class ExtCmd {
public:
    /**
     * This function is called when no internal commands with params.name were found,
     *      but external program with this name exists in current or global PATH
     *
     * @param params: name and args of the external program
     * @param env: current environment variables of the interpreter
     * @param input: input FILE stream for external program
     * @param output: output FILE stream for external program
     * @param err: error FILE stream for external program
     * @return return code of the external program
     */
    int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE* err) {
        std::filesystem::path current_path(env->at("PWD").to_string());
        if (current_path /= params.name; !FileUtils::is_file_exist(current_path)) {
            if (current_path = params.name; !FileUtils::is_file_exist(current_path)) {
                if (current_path = bp::search_path(params.name).string(); current_path == "") {
                    throw std::invalid_argument(params.name + ": command not found");
                }
            }
        }

        if (!params.args.empty()) {
            std::string seq_args = std::accumulate(std::begin(params.args) + 1, std::end(params.args), params.args[0],
                                       [](std::string s0, std::string const &s1) { return s0 += " " + s1; });
            return bp::system(current_path.string(), seq_args, *env, bp::std_out = output, bp::std_err = err, bp::std_in = input);
        } else {
            return bp::system(current_path.string(), *env, bp::std_out = output, bp::std_err = err, bp::std_in = input);
        }
    }
};

} // namespace commands

#endif //BASH_CLI_EXTCMD_HPP
