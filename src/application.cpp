#include "application.hpp"

#include <iostream>

void Application::run() {
    FILE* i_file = nullptr;
    FILE* o_file = nullptr;
    bool prevEOF = false;
    while (!handler->isExit()) {
        try {
            if (!prevEOF) {
                ios->greet();
            } else {
                prevEOF = false;
            }

            std::string user_input = ios->readLine();
            PipeLine pipeLine = analyzer->process(user_input);

            if (pipeLine.size() == 1) {
                lastReturnCode = handler->exec(pipeLine.front(), env, ios->getInput(), ios->getOutput());
            } else {
                i_file = tmpfile();
                for (size_t i = 0; i < pipeLine.size(); ++i) {
                    o_file = tmpfile();
                    if (i == 0) {
                        lastReturnCode |= handler->exec(pipeLine[i], env, ios->getInput(), o_file);
                    } else if (i == pipeLine.size() - 1) {
                        lastReturnCode |= handler->exec(pipeLine[i], env, i_file, ios->getOutput());
                    } else {
                        lastReturnCode |= handler->exec(pipeLine[i], env, i_file, o_file);
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
            ios->writeErrLine(e.what());
            lastReturnCode = 1;
        } catch (const EndOfGlobalInputStream& eof) {
            ios->resetInput();
            prevEOF = true;
        } catch (const std::exception& e) {
            ios->writeErrLine("Cannot execute command!");
            lastReturnCode = 1;
        }
        (*env)["?"] = std::to_string(lastReturnCode);
    }
}
