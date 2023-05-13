#pragma once

#include "file_utils.hpp"

#include <boost/process.hpp>

#include <numeric>

namespace Commands {

namespace bp = boost::process;

/**
 * Implementation of external command (not internal command)
 */
class ExtCmd {
public:
    ExtCmd() = default;
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
    int run(const token& params, std::shared_ptr<Environment> env, FileStream& input, FileStream& output, FileStream& err) const {
        std::filesystem::path tmp = env->current_path;
        if (tmp /= params.name; !fs::exists(tmp)) {
            if (tmp = params.name; !fs::exists(tmp)) {
                if (tmp = bp::search_path(params.name).string(); tmp == "") {
                    throw std::invalid_argument(params.name + ": command not found");
                }
            }
        }

        if (!params.args.empty()) {
            std::string seq_args = std::accumulate(std::begin(params.args) + 1, std::end(params.args), params.args[0],
                                       [](std::string s0, std::string const &s1) { return s0 += " " + s1; });
            return bp::system(tmp.string(), seq_args, env->vars, bp::std_out = output.to_FILE(), bp::std_err = err.to_FILE(), bp::std_in = input.to_FILE());
        } else {
            return bp::system(tmp.string(), env->vars, bp::std_out = output.to_FILE(), bp::std_err = err.to_FILE(), bp::std_in = input.to_FILE());
        }
    }
};

} // namespace commands
