#ifndef BASH_CLI_CD_HPP
#define BASH_CLI_CD_HPP

#include "cmd.hpp"
#include "file_utils.hpp"

#include <iostream>


namespace  commands {

/**
 * Implementation of echo command
 */
class Cd: public Cmd {
public:
    /**
     * Change the working directory
     * How it is written in interpreter syntax:  <cd path>
     *
     * <cd> without arguments leads to jumping to the root directory
     *  '..' in the parameters means to go to the parent directory
     *
     *  example:
     *  cd ..\\some_directory
     *
     *
     * @param params: token with command name in tok.name and command arguments in tok.args
     * @param env: current environment variables of the interpreter
     * @param input: input FILE stream (unused)
     * @param output: output FILE stream
     * @param err: error FILE stream
     * @return 0 when it was possible to change the directory,
     *         1 when errors occurred
     */
    virtual int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE* err) override {
        std::stringstream result, error;
        std::string path = env->at("PWD").to_string();

        //флаги не предусмотрены, поэтому параметров не более одного
        if(params.args.size() > 1) {
            error << "cd: too many arguments" << std::endl;
            FileUtils::writeToFile(error.str(), err);
            return 1;
        }

        //если без параметров -- переходим в рутовскую директорию
        if(params.args.size() == 0) {
            std::filesystem::path current_path(env->at("PWD").to_string());
            (*env)["PWD"] = current_path.root_path().string();
            result << current_path.root_path().string() << std::endl;
            FileUtils::writeToFile(result.str(), output);
            return 0;
        }

        std::filesystem::path current_path(env->at("PWD").to_string());
        std::string dir;
        std::string new_path = params.args[0];

        //путешествуем по введённому пути и проверяем на существование и на принадлежность к директории
        do {
            dir = new_path.substr(0, new_path.find_first_of('\\'));
            new_path = new_path.substr(new_path.find_first_of('\\') + 1);
            if(dir == new_path) {
                new_path = "";
            }

            //поднимаемся в родительскую директорию
            if(dir == "..") {
                current_path = current_path.parent_path();
            } else {
                current_path /= dir;

                //проверка, существует ли файл
                if(!FileUtils::is_file_exist(current_path)) {
                    error << "cd: " + params.args[0] + ": No such file or directory" << std::endl;
                    FileUtils::writeToFile(error.str(), err);
                    return 1;
                }

                //проверка, является ли файл директорией
                if(!FileUtils::is_directory_exist(current_path)) {
                    error << "cd: " + params.args[0] + ": Not a directory" << std::endl;
                    FileUtils::writeToFile(error.str(), err);
                    return 1;
                }
            }
        } while (new_path.size() != 0);

        (*env)["PWD"] = current_path.string();
        result << current_path.string() << std::endl;
        FileUtils::writeToFile(result.str(), output);
        return 0;
    }
};

} // namespace commands

#endif //BASH_CLI_CD_HPP
