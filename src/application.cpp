#include "application.hpp"

#include <iostream>

void Application::run() {
    FileStream i_file;
    FileStream o_file;
    while (!handler->isExit()) {
        lastReturnCode = 0;
        try {
            ios->greet();
            PipeLine pipeLine = analyzer->process(ios->readLine());

            i_file.reset();
            for (size_t i = 0; i < pipeLine.size(); ++i) {
                o_file.reset();
                FileStream& input_stream = (i == 0 ? ios->getInput() : i_file);
                FileStream& output_stream = (i + 1 == pipeLine.size() ? ios->getOutput() : o_file);
                lastReturnCode |= handler->exec(pipeLine[i], env, input_stream, output_stream);

                if (lastReturnCode) {
//                    fclose(o_file);
//                    o_file = tmpfile();
                    o_file.reset();
                }

                i_file.clear();
                o_file.rewind();
                i_file = std::move(o_file);
            }
            i_file.clear();

        } catch (const SyntaxExc& e) {
            ios->writeErr(e.what());
            lastReturnCode = 1;
        } catch (const EndOfInputStream& eof) {
            break;
        } catch (const std::exception& e) {
            ios->writeErr("Cannot execute command!\n");
            lastReturnCode = 1;
        }
        env->vars["?"] = std::to_string(lastReturnCode);
    }
    ios->getOutput().flush();
    ios->getErr().flush();
}
