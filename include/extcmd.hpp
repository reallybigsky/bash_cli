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
     * @param env: current environment of the interpreter
     * @param input: input FileStream for external program
     * @param output: output FileStream for external program
     * @param err: error FileStream for external program
     * @return return code of the external program
     */
    int run(const CmdToken& params, std::shared_ptr<Environment> env, FileStream& input, FileStream& output, FileStream& err) const {
        std::filesystem::path tmp = env->current_path;
        if (tmp /= params.name; !fs::exists(tmp)) {
            if (tmp = params.name; !fs::exists(tmp)) {
                if (tmp = bp::search_path(params.name).string(); tmp == "") {
                    throw std::invalid_argument(params.name + ": command not found");
                }
            }
        }

        return bp::system(tmp.string(), params.args, env->vars, bp::std_out = output.to_FILE(), bp::std_err = err.to_FILE(), bp::std_in = input.to_FILE());
    }
};

} // namespace commands
