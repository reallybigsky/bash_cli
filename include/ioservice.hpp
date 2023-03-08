#ifndef BASH_CLI_IOSERVICE_HPP
#define BASH_CLI_IOSERVICE_HPP

#include <iostream>
#include <string>
#include <fstream>

//TODO: documentation

class IOservice {
public:
    IOservice(int argc, char* argv[]) {
        for (size_t i = 1; i < argc; ++i) {
            std::string_view argument(argv[i]);
            std::string name;

            if (std::size_t pos = argument.find(inArg); pos != std::string_view::npos) {
                name = argument.substr(pos + inArg.size());
                input.open(name);
                if (!input.is_open()) {
                    throw std::invalid_argument("Cannot open input file \"" + name + '"');
                }
            } else if (pos = argument.find(outArg); pos != std::string_view::npos) {
                name = argument.substr(pos + outArg.size());
                output.open(name);
                if (!output.is_open()) {
                    throw std::invalid_argument("Cannot open output file \"" + name + '"');
                }
            } else if (pos = argument.find(errArg); pos != std::string_view::npos) {
                name = argument.substr(pos + errArg.size());
                err.open(name);
                if (!err.is_open()) {
                    throw std::invalid_argument("Cannot open error_output file \"" + name + '"');
                }
            } else {
                getOutput() << "bad arguments!" << std::endl;
            }
        }
    }

    std::istream& getInput() {
        if (input.is_open())
            return input;
        return std::cin;
    }

    std::ostream& getOutput() {
        if (output.is_open())
            return output;
        return std::cout;
    }

    std::ostream& getErr() {
        if (err.is_open())
            return err;
        return std::cerr;
    }

    void greet() {
        getOutput() << "> ";
    }

    std::string readLine() {
        std::string res;
        getline(getInput(), res);
        return res;
    }

private:
    const std::string inArg  = "in=" ;
    const std::string outArg = "out=";
    const std::string errArg = "err=";

    std::ifstream input;
    std::ofstream output;
    std::ofstream err;
};

#endif //BASH_CLI_IOSERVICE_HPP
