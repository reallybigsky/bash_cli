#include "handler.hpp"
#include "echo.hpp"
#include "pwd.hpp"
#include "cat.hpp"
#include "wc.hpp"
#include "grep.hpp"
#include "assignment.hpp"

Handler::CommandDict fill_commands() {
    Handler::CommandDict result;

    result.emplace("echo", std::make_unique<commands::Echo>());
    result.emplace("pwd", std::make_unique<commands::Pwd>());
    result.emplace("cat", std::make_unique<commands::Cat>());
    result.emplace("wc", std::make_unique<commands::Wc>());
    result.emplace("grep", std::make_unique<commands::Grep>());
    result.emplace("=", std::make_unique<commands::Assignment>());

    return result;
}

Handler::Handler(std::shared_ptr<IOservice> io)
    : exit(false)
    , ios(io)
    , commands(fill_commands())
{}

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
        ios->writeErrLine(ia.what());
        return 1;
    }

    return 0;
}
