#include "application.hpp"

#include <iostream>

void Application::exec_pipeline(const PipeLine& pipeline) {
    FileStream i_file;
    FileStream o_file;

    for (size_t i = 0; i < pipeline.size(); ++i) {
        FileStream& input_stream = (i == 0 ? ios->get_input() : i_file);
        FileStream& output_stream = (i + 1 == pipeline.size() ? ios->get_output() : o_file);
        last_return_code |= handler->exec(pipeline[i], env, input_stream, output_stream);

        if (last_return_code) {
            o_file.reset();
        }

        i_file.close_buffer();
        o_file.seek_begin();
        i_file = std::move(o_file);
        o_file.reset();
    }
}

void Application::run() {
    while (!handler->is_exit()) {
        last_return_code = 0;
        try {
            ios->greet();
            PipeLine pipeline = analyzer->process(ios->read_line());
            exec_pipeline(pipeline);
        } catch (const SyntaxExc& e) {
            ios->write_err(e.what());
            last_return_code = 1;
        } catch (const EndOfInputStream& eof) {
            break;
        } catch (const std::exception& e) {
            ios->write_err("Cannot execute command!\n");
            last_return_code = 1;
        }
        env->vars["?"] = std::to_string(last_return_code);
    }
    ios->get_output().flush();
    ios->get_err().flush();
}
