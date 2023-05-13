#include "application.hpp"

#include <iostream>

void Application::run() {
    FILE* i_file = nullptr;
    FILE* o_file = nullptr;
    while (!handler->isExit()) {
        lastReturnCode = 0;
        try {
            ios->greet();
            PipeLine pipeLine = analyzer->process(ios->readLine());

            i_file = tmpfile();
            for (size_t i = 0; i < pipeLine.size(); ++i) {
                o_file = tmpfile();
                FILE* input_stream = (i == 0? ios->getInput() : i_file);
                FILE* output_stream = (i + 1 == pipeLine.size() ? ios->getOutput() : o_file);
                lastReturnCode |= handler->exec(pipeLine[i], env, input_stream, output_stream);

                if (lastReturnCode) {
                    fclose(o_file);
                    o_file = tmpfile();
                }

                fclose(i_file);
                rewind(o_file);
                i_file = o_file;
            }
            fclose(i_file);

        } catch (const SyntaxExc& e) {
            ios->writeErrLine(e.what());
            lastReturnCode = 1;
        } catch (const EndOfGlobalInputStream& eof) {
            break;
        } catch (const std::exception& e) {
            ios->writeErrLine("Cannot execute command!");
            lastReturnCode = 1;
        }
        env->vars["?"] = std::to_string(lastReturnCode);
    }
    ios->flushOutput();
    ios->flushErr();
}
