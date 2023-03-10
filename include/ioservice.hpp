#ifndef BASH_CLI_IOSERVICE_HPP
#define BASH_CLI_IOSERVICE_HPP

#include "common.hpp"

#include <string>
#include <exception>
#include <stdexcept>

//TODO: documentation

class IOservice {
public:
    IOservice(int argc, char* argv[])
        : f_input(nullptr)
        , f_output(nullptr)
        , f_err(nullptr)
    {
        for (size_t i = 1; i < argc; ++i) {
            std::string argument(argv[i]);
            std::string name;

            if (std::size_t pos = argument.find(inArg); pos != std::string_view::npos) {
                name = argument.substr(pos + inArg.size());
                f_input = fopen(name.c_str(), "r");
                if (!f_input) {
                    throw std::invalid_argument("Cannot open input file: \"" + name + '"');
                }
            } else if (pos = argument.find(outArg); pos != std::string_view::npos) {
                name = argument.substr(pos + outArg.size());
                f_output = fopen(name.c_str(), "w");
                if (!f_output) {
                    throw std::invalid_argument("Cannot open output file: \"" + name + '"');
                }
            } else if (pos = argument.find(errArg); pos != std::string_view::npos) {
                name = argument.substr(pos + errArg.size());
                f_err = fopen(name.c_str(), "w");
                if (!f_err) {
                    throw std::invalid_argument("Cannot open error_output: file \"" + name + '"');
                }
            } else {
                throw std::invalid_argument("Unknown argument: \"" + argument + '"');
            }
        }
    }

    ~IOservice() {
        if (f_input)
            fclose(f_input);
        if (f_output)
            fclose(f_output);
        if (f_err)
            fclose(f_err);
    }

    FILE* getInput() const {
        if (f_input)
            return f_input;
        return stdin;
    }

    FILE* getOutput() const {
        if (f_output)
            return f_output;
        return stdout;
    }

    FILE* getErr() const {
        if (f_err)
            return f_err;
        return stderr;
    }

    void write(const std::string& str) const {
        if (writeToFile(str, getOutput())) {
            throw std::runtime_error("Cannot write to out!");
        }
    }

    void writeErr(const std::string& str) const {
        if (writeToFile(str, getErr())) {
            throw std::runtime_error("Cannot write to err!");
        }
    }

    void greet() const {
        write("> ");
    }

    std::string readLine() const {
        char* line = nullptr;
        size_t cnt = 0;
        getline(&line, &cnt, getInput());
        if (!line)
            return "";
        std::string res(line);
        free(line);
        return res;
    }

private:
    const std::string inArg  = "in=" ;
    const std::string outArg = "out=";
    const std::string errArg = "err=";

    FILE* f_input;
    FILE* f_output;
    FILE* f_err;
};

#endif //BASH_CLI_IOSERVICE_HPP
