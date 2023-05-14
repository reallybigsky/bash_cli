#pragma once

#include "file_utils.hpp"
#include "cmd.hpp"


namespace Commands {

/**
 * Implementation of cat command
 */
class Cat : public Cmd {
public:
    /**
     * Concatenates params.args[0], params.args[1], ..., params.args[n] and outputs to the output stream
     * How it is written in interpreter syntax:  <cat file1 file2 ...>
     *
     * Absence of params.args is valid, however the read is from the incoming stream:
     * cat some text in console
     * some text in console
     *
     * @param params: CmdToken with command name in params.name and command arguments in params.args
     * @param env: current environment of the interpreter
     * @param input: input FileStream
     * @param output: output FileStream
     * @param err: error FileStream
     * @return 0 if there were no errors, 1 otherwise
     *
     */
    virtual int run(const CmdToken& params, std::shared_ptr<Environment> env, FileStream& input, FileStream& output, FileStream& err) const override {
        if (params.args.empty()) {
            while (auto line = input.read_line()) {
                output << line.value();
            }
            return 0;
        }

        std::stringstream result;
        size_t error_count = 0;
        for (auto& filename: params.args) {
            std::filesystem::path filepath = env->current_path / filename;

            // проверка на то, существует ли файл в текущей директории
            if (!fs::exists(filepath)) {
                if (!fs::exists(filename)) {
                    ++error_count;
                    result << params.name << ": " << filename << ": No such file or directory" << std::endl;
                    continue;
                }
                filepath = filename;
            }

            // проверка на то, можно ли открыть файл на чтение
            if (!std::ifstream(filepath).is_open()) {
                ++error_count;
                result << filename << ": Permission denied" << std::endl;
                continue;
            }
            result << get_file_contents(filepath);
        }

        if (error_count == params.args.size()) {
            err << result.str();
            return 1;
        }

        output << result.str();

        if (error_count > 0) {
            return 1;
        }

        return 0;
    }

private:

    static std::string get_file_contents(const std::filesystem::path& filename) {
        std::ifstream ifs(filename);
        std::string result;

        ifs.seekg(0, std::ios::end);
        result.reserve(ifs.tellg());
        ifs.seekg(0, std::ios::beg);

        result.assign((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
        return result;
    }
};

} // namespace commands
