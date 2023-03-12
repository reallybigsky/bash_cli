#ifndef BASH_CLI_ASSIGNMENT_HPP
#define BASH_CLI_ASSIGNMENT_HPP

#include "cmd.hpp"

#include <regex>

namespace commands {

/**
 * Assignment command '=' in REPL interpreter
 */
class Assignment : public Cmd {
public:
    /**
     * Assigns tok.args[1] to tok.args[0]
     * How it is written in interpreter syntax:  <tok.args[0]>=<tok.args[1]>
     *
     * Absence of <tok.args[1]> is valid:
     * <tok.args[0]>=
     * Then empty string will be assigned to tok.args[0]
     *
     * All arguments after tok.args[1] are ignored
     *
     * @param tok token with command name in tok.name and command arguments in tok.args
     * @param env current environment variables of REPL interpreter
     * @param input input FILE stream (unused)
     * @param output output FILE stream (unused)
     * @param err error FILE stream
     * @return 0 if there were no errors, 1 otherwise
     *
     * @throws std::invalid_argument Thrown if tok.args is empty, in other words if there is no left operand of '='
     */
    virtual int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE* err) override {
        if (params.args.empty())
            throw std::invalid_argument("Assignment with no arguments!");

        std::stringstream result;
        if (params.args.size() > 1) {
            std::regex spec_symbols(R"('|"|\\|\$)");
            result << std::regex_replace(params.args[1], spec_symbols, "\\$&");
        } else {
            result << "";
        }

        (*env)[params.args[0]] = result.str();
        return 0;
    }
};

} // namespace commands


#endif //BASH_CLI_ASSIGNMENT_HPP
