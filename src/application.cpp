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
            std::vector<job> after_parse = preprocess::runParser(after_lex);

            if (after_parse.size() > 1) {
                std::stringstream ss;
                handler.handle(after_parse.front(), envState, ios.getInput(), ss);

                for (size_t i = 1; i < after_parse.size() - 1; ++i) {
                    std::stringstream out;
                    handler.handle(after_parse[i], envState, ss, out);
                    ss = std::move(out);
                }

                handler.handle(after_parse.back(), envState, ss, ios.getOutput());
            } else if (after_parse.size() == 1) {
                handler.handle(after_parse.front(), envState, ios.getInput(), ios.getOutput());
            } else {
                // WTF!?
            }

        } catch (...) {
            std::cout << "caught exception, aborting..." << std::endl;

            break;
        }
    }

}