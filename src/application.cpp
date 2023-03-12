#include "application.hpp"

void Application::run() {
    FILE* i_file = nullptr;
    FILE* o_file = nullptr;
    while (!handler->isExit()) {
        try {
            ios->greet();
            std::string user_input = ios->readLine();
            PipeLine after_parse = Parser::process(user_input);

            if (after_parse.size() == 1) {
                lastReturnCode = handler->exec(after_parse.front(), env, ios->getInput(), ios->getOutput());
            } else {
                i_file = tmpfile();
                for (size_t i = 0; i < after_parse.size(); ++i) {
                    o_file = tmpfile();
                    if (i == 0) {
                        lastReturnCode = handler->exec(after_parse[i], env, ios->getInput(), o_file);
                    } else if (i == after_parse.size() - 1) {
                        lastReturnCode = handler->exec(after_parse[i], env, i_file, ios->getOutput());
                        fclose(i_file);
                        fclose(o_file);
                        break;
                    } else {
                        lastReturnCode = handler->exec(after_parse[i], env, i_file, o_file);
                    }
                    fclose(i_file);
                    rewind(o_file);
                    i_file = o_file;
                }
            }
        } catch (const SyntaxExc& e) {
            ios->writeLine(e.what());
        } catch (...) {
            if (i_file)
                fclose(i_file);
            if (o_file)
                fclose(o_file);
            ios->writeLine("Caught unknown exception, abort... \n");
            break;
        }
    }
}