#include "handler.hpp"
#include "echo.hpp"
#include "pwd.hpp"
#include "cat.hpp"
#include "wc.hpp"
#include "grep.hpp"
#include "assignment.hpp"

typedef std::unordered_map<std::string, std::unique_ptr<Cmd>> CommandDict;

CommandDict fill_commands() {
    CommandDict result;

    result.emplace("echo", std::make_unique<Commands::Echo>());
    result.emplace("pwd", std::make_unique<Commands::Pwd>());
    result.emplace("cat", std::make_unique<Commands::Cat>());
    result.emplace("wc", std::make_unique<Commands::Wc>());
    result.emplace("grep", std::make_unique<Commands::Grep>());
    result.emplace("=", std::make_unique<Commands::Assignment>());

    return result;
}

const CommandDict commands = fill_commands();
const Commands::ExtCmd extCmd;

int Handler::exec(const token& tok, std::shared_ptr<Environment> env, FileStream& i_file, FileStream& o_file) {
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
        ios->writeErr(ia.what());
        return 1;
    }

    return 0;
}
