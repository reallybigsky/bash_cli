#include "handler.hpp"
#include "echo.hpp"

namespace bp = boost::process;

Handler::Handler(std::shared_ptr<IOservice> ioserv)
    : ios(ioserv)
    {
    // добавление новой команды
    commands = {
            {"echo", std::make_shared<Echo>()},
//            {"pwd", std::make_shared<pwdImpl>()},
//            {"cat", std::make_shared<catImpl>()},
//            {"wc", std::make_shared<wcImpl>()},
//            {"exit", std::make_shared<exitImpl>()},
//            {"=", std::make_shared<assignImpl>()}
    };
}


int Handler::exec(const token& j, std::shared_ptr<Environment> env, FILE* i_file, FILE* o_file) {
//TODO: exceptions and errors

    if (commands.contains(j.name)) {
        commands[j.name]->run(j, env, i_file, o_file);
    }
    else {
        extCmd.run(j, env, i_file, o_file);
    }

    return 0;
}
