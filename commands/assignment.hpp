#pragma once

#include "cmd.hpp"

#include <regex>
#include <unordered_set>

namespace Commands {

/**
 * Implementation of assignment command '='
 */
class Assignment : public Cmd {
public:
    /**
     * Assign params.args[1] to params.args[0]
     * How it is written in the interpreter syntax:  <params.args[0]>=<params.args[1]>
     *
     * Absence of <params.args[1]> is valid:
     * <params.args[0]>=
     * Then empty string will be assigned to params.args[0]
     *
     * All arguments after params.args[1] are ignored
     *
     * @param params: CmdToken with command name in params.name and command arguments in params.args
     * @param env: current environment of the interpreter
     * @param input: input FileStream (unused)
     * @param output: output FileStream (unused)
     * @param err: error FileStream
     * @return 0 if there were no errors, 1 otherwise
     *
     */
    virtual int run(const CmdToken& params, std::shared_ptr<Environment> env, FileStream&, FileStream&, FileStream& err) const override {
        if (params.args.empty()) {
            err << "Assignment with no arguments!\n";
            return 1;
        }

        if (params.args.size() > 1) {
            static const std::regex spec_symbols(R"('|"|\\|\$)");
            env->vars[params.args[0]] = std::regex_replace(params.args[1], spec_symbols, "\\$&");
        } else {
            env->vars[params.args[0]] = "";
        }

        return 0;
    }
};

} // namespace commands
