#include "application.hpp"
#include "preprocess.hpp"

void Application::run() {
    //TODO: exceptions and errors
    //TODO: exit?

    FILE* i_file = nullptr;
    FILE* o_file = nullptr;
    while (true) {
        try {
            ios->greet();
            std::string user_input = ios->readLine();

            PipeLine after_parse = Parser::process(user_input);

            if (after_parse.size() == 1) {
                handler->exec(after_parse.front(), vars, ios->getInput(), ios->getOutput());
            } else {
//                FILE* i_file = tmpfile();
                i_file = tmpfile();
                for (size_t i = 0; i < after_parse.size(); ++i) {
//                    FILE* o_file = tmpfile();
                    o_file = tmpfile();

                    assert(i_file);
                    assert(o_file);

                    if (i == 0) {
                        handler->exec(after_parse[i], vars, ios->getInput(), o_file);
                    } else if (i == after_parse.size() - 1) {
                        handler->exec(after_parse[i], vars, i_file, ios->getOutput());
                        fclose(i_file);
                        fclose(o_file);
                        break;
                    } else {
                        handler->exec(after_parse[i], vars, i_file, o_file);
                    }
                    fclose(i_file);
                    rewind(o_file);
                    i_file = o_file;
                }
            }
        } catch (...) {
            fclose(i_file);
            fclose(o_file);
            ios->write("caught exception, aborting...\n");
            break;
        }
    }

}