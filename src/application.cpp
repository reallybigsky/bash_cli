#include "application.hpp"
#include "preprocess.hpp"

#include <string>
#include <sstream>

void Application::run() {
    //TODO: exceptions and errors
    //TODO: exit?
    while (true) {
        try {
            ios.greet();
            std::string user_input = ios.readLine();

            std::vector<std::string> after_lex = preprocess::runLexer(user_input);
            std::vector<msg> after_parse = preprocess::runParser(after_lex);

            if (after_parse.size() > 1) {
                std::stringstream ss;
                handler.handleMsg(after_parse.front(), {path, varEnv, ios.getInput(), ss});

                for (size_t i = 1; i < after_parse.size() - 1; ++i) {
                    std::stringstream out;
                    handler.handleMsg(after_parse[i], {path, varEnv, ss, out});
                    ss = std::move(out);
                }

                handler.handleMsg(after_parse.back(), {path, varEnv, ss, ios.getOutput()});
            } else if (after_parse.size() == 1) {
                handler.handleMsg(after_parse.front(), {path, varEnv, ios.getInput(), ios.getOutput()});
            } else {
                // WTF!?
            }

        } catch (...) {
            std::cout << "caught exception, aborting..." << std::endl;

            break;
        }
    }

}