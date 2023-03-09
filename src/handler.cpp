#include "handler.hpp"
#include "echo.hpp"

namespace bp = boost::process;

Handler::Handler() {
    // добавление новой команды
    commands = {
//            {"echo", std::make_shared<Echo>()},
//            {"pwd", std::make_shared<pwdImpl>()},
//            {"cat", std::make_shared<catImpl>()},
//            {"wc", std::make_shared<wcImpl>()},
//            {"exit", std::make_shared<exitImpl>()},
//            {"=", std::make_shared<assignImpl>()}
    };
}


int Handler::exec(const job& j, const EnvState& envState) {
//TODO: exceptions and errors

    if (commands.contains(j.name)) {
        std::istream* input = envState.ipsCurr.get();
        std::ostream* output = envState.opsCurr.get();

        std::ostream& err = envState.ios->getErr();

        if (envState.cmdPos == CmdPos::first) {
            input = &envState.ios->getInput();
        }
        if (envState.cmdPos == CmdPos::last) {
            output = &envState.ios->getOutput();
        }

        commands[j.name]->run(j, envState.vars, *input, *output, err);
        envState.ios->updateFilesPos();
    }
    else {
        extCmd.run(j, envState);
        envState.ios->updateStreamsPos();
    }

    return 0;
}
