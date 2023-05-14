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

int Handler::exec(const CmdToken& token, std::shared_ptr<Environment> env, FileStream& input, FileStream& output) {
    try {
        if (token.name.empty()) {
            return 0;
        } else if (token.name == "exit") {
            exit = true;
        } else if (commands.contains(token.name)) {
            return commands.at(token.name)->run(token, env, input, output, ios->get_err());
        } else {
            return extCmd.run(token, env, input, output, ios->get_err());
        }
    } catch (const std::invalid_argument& ia) {
        ios->write_err(ia.what());
        return 1;
    }

    return 0;
}
