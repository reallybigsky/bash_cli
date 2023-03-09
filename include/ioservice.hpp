#ifndef BASH_CLI_IOSERVICE_HPP
#define BASH_CLI_IOSERVICE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <memory>

//TODO: documentation

class IOservice {
public:
    IOservice(int argc, char* argv[])
        : f_input(nullptr)
        , f_output(nullptr)
        , f_err(nullptr)
    {
        for (size_t i = 1; i < argc; ++i) {
            std::string_view argument(argv[i]);
            std::string name;

            if (std::size_t pos = argument.find(inArg); pos != std::string_view::npos) {
                name = argument.substr(pos + inArg.size());
                input.open(name);
                f_input = fopen(name.c_str(), "r");
                if (!input.is_open() || !f_input) {
                    throw std::invalid_argument("Cannot open input file \"" + name + '"');
                }
            } else if (pos = argument.find(outArg); pos != std::string_view::npos) {
                name = argument.substr(pos + outArg.size());
                output.open(name);
                f_output = fopen(name.c_str(), "w");
                if (!output.is_open() || !f_output) {
                    throw std::invalid_argument("Cannot open output file \"" + name + '"');
                }
            } else if (pos = argument.find(errArg); pos != std::string_view::npos) {
                name = argument.substr(pos + errArg.size());
                err.open(name);
                f_err = fopen(name.c_str(), "w");
                if (!err.is_open() ||  !f_err) {
                    throw std::invalid_argument("Cannot open error_output file \"" + name + '"');
                }
            } else {
                getOutput() << "bad arguments!" << std::endl;
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

    std::istream& getInput() {
        if (input.is_open())
            return input;
        return std::cin;
    }

    FILE* getInputFile() {
        if (f_input)
            return f_input;
        return stdin;
    }

    std::ostream& getOutput() {
        if (output.is_open())
            return output;
        return std::cout;
    }

    FILE* getOutputFile() {
        if (f_output)
            return f_output;
        return stdout;
    }

    std::ostream& getErr() {
        if (err.is_open())
            return err;
        return std::cerr;
    }

    FILE* getErrFile() {
        if (f_err)
            return f_err;
        return stderr;
    }

    void greet() {
        getOutput() << "> ";
        getOutput().flush();
    }

    std::string readLine() {
        std::string res;
        char* line = nullptr;
        if (!f_input) {
            size_t cnt = 0;
            getline(&line, &cnt, stdin);
        }
        free(line);
        getline(input, res);
        if (f_input) {
            fseek(f_input, res.size(), SEEK_CUR);
        }
        return res;
    }

    void writeLine(const std::string& str) {
        output << str << std::endl;
        if (f_output) {
            fseek(f_output, str.size(), SEEK_CUR);
        }
    }

    void writeErrLine(const std::string& str) {
        err << str << std::endl;
        if (f_err) {
            fseek(f_err, str.size(), SEEK_CUR);
        }
    }

    void updateFilesPos() {
        if (f_input) {
            fseek(f_input, input.tellg(), SEEK_SET);
        }
        if (f_output) {
            fseek(f_output, output.tellp(), SEEK_SET);
        }
        if (f_err) {
            fseek(f_err, err.tellp(), SEEK_SET);
        }
    }

    void updateStreamsPos() {
        if (input.is_open() && f_input) {
            input.seekg(ftell(f_input));
        }
        if (output.is_open() && f_output) {
            output.seekp(ftell(f_output));
        }
        if (err.is_open() && f_err) {
            err.seekp(ftell(f_err));
        }
    }

private:
    const std::string inArg  = "in=" ;
    const std::string outArg = "out=";
    const std::string errArg = "err=";

    FILE* f_input;
    FILE* f_output;
    FILE* f_err;

    std::ifstream input;
    std::ofstream output;
    std::ofstream err;
};

#endif //BASH_CLI_IOSERVICE_HPP
