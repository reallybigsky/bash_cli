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


int Handler::handle(const job& j, EnvState& envState, std::istream& is, std::ostream& os) {
//TODO: exceptions and errors
    if (commands.contains(j.name)) {
        commands[j.name]->run(j, envState, is, os);
    }
    // поиск внешней программы
    // либо ошибка - неизвестная команда

    return 0;
}
