#ifndef BASH_CLI_IOSERVICE_HPP
#define BASH_CLI_IOSERVICE_HPP

#include "common.hpp"
#include "file_utils.hpp"

#include <string>
#include <exception>
#include <stdexcept>

/**
 * EndOfInputStream exception
 */
struct EndOfGlobalInputStream : public std::ios_base::failure {
    EndOfGlobalInputStream(const std::string& str) : std::ios_base::failure(str) {}
};


/**
 * Class that provides access to global IO streams of the interpreter
 *
 * IO streams are represented as C FILE* streams
 */
class IOservice {
public:
    IOservice(int argc, const char* argv[])
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
                writeErrLine("Unknown argument: \"" + argument + '"');
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

    /**
     * @return global Input FILE stream
     */
    FILE* getInput() const {
        if (f_input)
            return f_input;
        return stdin;
    }

    /**
     * @return global Output FILE stream
     */
    FILE* getOutput() const {
        if (f_output)
            return f_output;
        return stdout;
    }

    /**
     * @return global Error FILE stream
     */
    FILE* getErr() const {
        if (f_err)
            return f_err;
        return stderr;
    }

    /**
     * Write string in global Output stream
     *
     * @param str: string to be written
     *
     * @throws std::runtime_error: if there was an error while writing
     */
    void write(const std::string& str) const {
        if (FileUtils::writeToFile(str, getOutput())) {
            throw std::runtime_error("Cannot write to out!");
        }
    }

    /**
     * Append newline symbol to string and write it in global Output stream
     *
     * @param str: string to be appended with newline and then written
     *
     * @throws std::runtime_error: if there was an error while writing
     */
    void writeLine(const std::string& str) const {
        write(str + '\n');
    }

    /**
     * Write string in global Error stream
     *
     * @param str: string to be written
     *
     * @throws std::runtime_error: if there was an error while writing
     */
    void writeErr(const std::string& str) const {
        if (FileUtils::writeToFile(str, getErr())) {
            throw std::runtime_error("Cannot write to err!");
        }
    }

    /**
     * Append newline symbol to string and write it in global Error stream
     *
     * @param str: string to be appended with newline and then written
     *
     * @throws std::runtime_error: if there was an error while writing
     */
    void writeErrLine(const std::string& str) const {
        writeErr(str + '\n');
    }

    /**
     * Write greeting to the global Output stream
     */
    void greet() const {
        write(greeting);
    }

    /**
     * read entire line from global Input stream
     *
     * @return string line from global Input stream
     *
     * @throws EndOfInputStream: if end of Input stream was reached
     */
    std::string readLine() const {
        auto str = FileUtils::readLine(getInput());
        if (!str) {
            throw EndOfGlobalInputStream("");
        }
        return str.value();
    }

    /**
     * clear error from global input stream for next user input
     */
    void resetInput() {
        if (f_input)
            clearerr(f_input);
        else
            clearerr(stdin);
    }

private:
    const std::string greeting = "> ";

    const std::string inArg  = "in=" ;
    const std::string outArg = "out=";
    const std::string errArg = "err=";

    FILE* f_input;
    FILE* f_output;
    FILE* f_err;
};

#endif //BASH_CLI_IOSERVICE_HPP
