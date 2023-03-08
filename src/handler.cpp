#include "handler.hpp"
#include "echo.hpp"
#include "pwd.hpp"
#include "cat.hpp"
#include "wc.hpp"
#include "assignment.hpp"

Handler::Handler() {
    // добавление новой команды
    commands = {
            {"echo", std::make_shared<commands::Echo>()},
            {"pwd", std::make_shared<commands::Pwd>()},
            {"cat", std::make_shared<commands::Cat>()},
            {"wc", std::make_shared<commands::Wc>()},
            //{"exit", std::make_shared<exitImpl>()},
            {"=", std::make_shared<commands::Assignment>()}
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
