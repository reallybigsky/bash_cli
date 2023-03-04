#include "handler.hpp"
#include "echo.hpp"

Handler::Handler() {
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


int Handler::handleMsg(const msg& m, EnvState es) {
//TODO: exceptions and errors
    if (commands.contains(m.name)) {
        commands[m.name]->run(m, es);
    }
    // поиск внешней программы
    // либо ошибка - неизвестная команда

    return 0;
}
