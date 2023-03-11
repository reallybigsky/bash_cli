#ifndef BASH_CLI_CAT_HPP
#define BASH_CLI_CAT_HPP

#include "cmd.hpp"
#include "commands_utils.hpp"


namespace commands {

class Cat : public Cmd {
public:

    virtual int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE* err) override {
        std::stringstream result;
        int32_t error_count = 0;

        if (params.args.empty()) {
            while (auto line = FileUtils::readLine(input))
                result << line.value() << std::endl;

            FileUtils::writeToFile(result.str(), output);
            return 0;
        }

        for (auto &filename: params.args) {
            fs::path current_path(env->at("PWD").to_string());
            current_path /= filename;

            //проверка на то, существует ли файл в текущей директории
            if (!is_file_exist(current_path)) {
                if (!is_file_exist(filename)) {
                    ++error_count;
                    result << params.name << ": " << filename << ": No such file or directory" << std::endl;
                    continue;
                }
                current_path = filename;
            }

            //проверка на то, можно ли открыть файл на чтение
            if (!is_readable(current_path)) {
                ++error_count;
                result << filename << ": Permission denied" << std::endl;
                continue;
            }
            result << get_file_contents(current_path);
        }

        if (error_count == params.args.size())
            throw std::invalid_argument(result.str());

        FileUtils::writeToFile(result.str(), output);
        if (error_count > 0)
            return 1;
        return 0;
    }

private:

    static std::string get_file_contents(const fs::path &filename) {
        std::ifstream t(filename);
        std::string result;

        t.seekg(0, std::ios::end);
        result.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        result.assign((std::istreambuf_iterator<char>(t)),
                      std::istreambuf_iterator<char>());

        return result;
    }
};

} // namespace commands

#endif //BASH_CLI_CAT_HPP
