#include "application.hpp"

#include <iostream>

void Application::run() {
    FILE* i_file = nullptr;
    FILE* o_file = nullptr;
    while (!handler->isExit()) {
        try {
            ios->greet();
            std::string user_input = ios->readLine();
            PipeLine pipeLine = analyzer->process(user_input);

            if (pipeLine.size() == 1) {
                lastReturnCode = handler->exec(pipeLine.front(), env, ios->getInput(), ios->getOutput());
            } else {
                i_file = tmpfile();
                for (size_t i = 0; i < pipeLine.size(); ++i) {
                    o_file = tmpfile();
                    if (i == 0) {
                        lastReturnCode = handler->exec(pipeLine[i], env, ios->getInput(), o_file);
                    } else if (i == pipeLine.size() - 1) {
                        lastReturnCode = handler->exec(pipeLine[i], env, i_file, ios->getOutput());
                    } else {
                        lastReturnCode = handler->exec(pipeLine[i], env, i_file, o_file);
                    }

                    if (lastReturnCode) {
                        fclose(o_file);
                        o_file = tmpfile();
                    }

                    fclose(i_file);
                    rewind(o_file);
                    i_file = o_file;
                }
                fclose(i_file);
            }
        } catch (const SyntaxExc& e) {
            ios->writeLine(e.what());
        } catch (const EndOfInputStream& eof) {
            break;
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
