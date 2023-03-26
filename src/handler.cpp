#include "handler.hpp"
#include "echo.hpp"
#include "pwd.hpp"
#include "cat.hpp"
#include "wc.hpp"
#include "grep.hpp"
#include "assignment.hpp"

Handler::Handler(std::shared_ptr<IOservice> io)
    : exit(false)
    , ios(io)
{
    /// make cmd executable
    commands = {
        {"echo", std::make_shared<commands::Echo>()},
        {"pwd", std::make_shared<commands::Pwd>()},
        {"cat", std::make_shared<commands::Cat>()},
        {"wc", std::make_shared<commands::Wc>()},
        {"grep", std::make_shared<commands::Grep>()},
        {"=", std::make_shared<commands::Assignment>()}
    };
}

int Handler::exec(const token& tok, std::shared_ptr<Environment> env, FILE* i_file, FILE* o_file) {
    try {
        if (tok.name.empty()) {
            return 0;
        } else if (tok.name == "exit") {
            exit = true;
        } else if (commands.contains(tok.name)) {
            return commands.at(tok.name)->run(tok, env, i_file, o_file, ios->getErr());
        } else {
            return extCmd.run(tok, env, i_file, o_file, ios->getErr());
        }
    } catch (const std::invalid_argument& ia) {
        return 1;
    }

    return 0;
}
