#include "application.hpp"

#include <iostream>

void Application::run() {
    FileStream i_file;
    FileStream o_file;
    while (!handler->is_exit()) {
        last_return_code = 0;
        try {
            ios->greet();
            PipeLine pipeline = analyzer->process(ios->readLine());

            i_file.reset();
            for (size_t i = 0; i < pipeline.size(); ++i) {
                o_file.reset();
                FileStream& input_stream = (i == 0 ? ios->getInput() : i_file);
                FileStream& output_stream = (i + 1 == pipeline.size() ? ios->getOutput() : o_file);
                last_return_code |= handler->exec(pipeline[i], env, input_stream, output_stream);

                if (last_return_code) {
                    o_file.reset();
                }

                i_file.clear();
                o_file.rewind();
                i_file = std::move(o_file);
            }
            i_file.clear();

        } catch (const SyntaxExc& e) {
            ios->writeErr(e.what());
            last_return_code = 1;
        } catch (const EndOfInputStream& eof) {
            break;
        } catch (const std::exception& e) {
            ios->writeErr("Cannot execute command!\n");
            last_return_code = 1;
        }
        env->vars["?"] = std::to_string(last_return_code);
    }
    ios->getOutput().flush();
    ios->getErr().flush();
}
