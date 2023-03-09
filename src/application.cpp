#include "application.hpp"
#include "preprocess.hpp"

void Application::run() {
    //TODO: exceptions and errors
    //TODO: exit?
    while (true) {
        try {
            ios->greet();
            std::string user_input = ios->readLine();

            std::vector<std::string> after_lex = preprocess::runLexer(user_input);
            std::vector<job> after_parse = preprocess::runParser(after_lex);

            if (after_parse.size() == 1) {
                handler->exec(after_parse.front(), vars, ios->getInput(), ios->getOutput());
            } else {
                FILE* i_file = tmpfile();
                for (size_t i = 0; i < after_parse.size(); ++i) {
                    FILE* o_file = tmpfile();

                    assert(i_file);
                    assert(o_file);

                    if (i == 0) {
                        handler->exec(after_parse[i], vars, ios->getInput(), o_file);
                    } else if (i == after_parse.size() - 1) {
                        handler->exec(after_parse[i], vars, i_file, ios->getOutput());
                        fclose(o_file);
                    } else {
                        handler->exec(after_parse[i], vars, i_file, o_file);
                    }
                    fclose(i_file);
                    i_file = o_file;
                }
            }
        } catch (...) {
            ios->write("caught exception, aborting...\n");
            break;
        }
    }

}