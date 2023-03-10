#ifndef BASH_CLI_EXTCMD_HPP
#define BASH_CLI_EXTCMD_HPP

#include <numeric>
#include "commands_utils.hpp"
#include <boost/process.hpp>

namespace commands {

    namespace bp = boost::process;

    class ExtCmd {
    public:
        int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output) {

            fs::path current_path((*env)["PATH"].to_string());
            if (current_path.replace_filename((*env)["PATH"].to_string()); !is_file_exist(current_path)) {
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
            int result = bp::system(current_path.string(), seq_args, *env, bp::std_out = input, bp::std_in = output);
            return result;
        }
    };
}

#endif //BASH_CLI_EXTCMD_HPP
