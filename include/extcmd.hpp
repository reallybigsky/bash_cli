#ifndef BASH_CLI_EXTCMD_HPP
#define BASH_CLI_EXTCMD_HPP

#include <numeric>
#include "commands_utils.hpp"

namespace commands {

    class ExtCmd {
    public:
        int run(const job &params, EnvState &env, bp::pstream &in, bp::pstream &out) {

            fs::path current_path(env.varEnv["PATH"].to_string());
            if (current_path.replace_filename(env.varEnv["PATH"].to_string()); !is_file_exist(current_path)) {
                if (current_path = params.name; !is_file_exist(current_path)) {
                    if (current_path = bp::search_path(params.name).string(); current_path == "") {
                        throw std::invalid_argument(params.name + std::string(": command not found"));
                    }
                }
            }

            std::string seq_args;
            if (!params.args.empty())
                seq_args = std::accumulate(std::begin(params.args) + 1, std::end(params.args), params.args[0],
                                           [](std::string s0, std::string const &s1) { return s0 += " " + s1; });

            // добавить std_err
            int result = bp::system(current_path.string(), seq_args, env.varEnv, bp::std_out = in, bp::std_in < out);
            return result;
        }
    };
}

#endif //BASH_CLI_EXTCMD_HPP
