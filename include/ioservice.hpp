#pragma once

#include "common.hpp"
#include "file_utils.hpp"

#include <string>
#include <exception>
#include <stdexcept>
#include <utility>
#include <span>


/**
 * Class that provides access to global IO streams of the interpreter
 *
 * IO streams are represented as C FILE* streams
 */
class IOService {
public:
    IOService(size_t argc, std::span<const char*> argv, std::string gr)
        : greeting(std::move(gr))
        , f_input(stdin)
        , f_output(stdout)
        , f_err(stderr)
    {
        for (size_t i = 1; i < argc; ++i) {
            std::string_view argument(argv[i]);
            std::string name;

            if (std::size_t pos = argument.find(inArg); pos != std::string_view::npos) {
                name = argument.substr(pos + inArg.size());
                f_input = FileStream(name, "r");
            } else if (pos = argument.find(outArg); pos != std::string_view::npos) {
                name = argument.substr(pos + outArg.size());
                f_output = FileStream(name, "w");
            } else if (pos = argument.find(errArg); pos != std::string_view::npos) {
                name = argument.substr(pos + errArg.size());
                f_err = FileStream(name, "w");
            } else {
                f_err << "Unknown argument: \"" + std::string(argument) + '"';
            }
        }
    }

    /**
     * @return global Input FileStream
     */
    FileStream& get_input() {
        return f_input;
    }

    /**
     * @return global Output FileStream
     */
    FileStream& get_output() {
        return f_output;
    }

    /**
     * @return global Error FileStream
     */
    FileStream& get_err() {
        return f_err;
    }

    /**
     * Write string in global Output stream
     *
     * @param str: string to be written
     *
     * @throws std::runtime_error: if there was an error while writing
     */
    void write(const std::string& str) {
        f_output << str;
    }

    /**
     * Write string in global Error stream
     *
     * @param str: string to be written
     *
     * @throws std::runtime_error: if there was an error while writing
     */
    void write_err(const std::string& str) {
        f_err << str;
    }

    /**
     * Write greeting to the global Output stream
     */
    void greet() {
        f_output << greeting;
    }

    /**
     * read entire line from global Input stream
     *
     * @return string line from global Input stream
     *
     * @throws EndOfInputStream: if end of Input stream was reached
     */
    std::string read_line() const {
        auto str = f_input.read_line();
        if (!str) {
            throw EndOfInputStream("");
        }
        return std::move(str.value());
    }

private:
    const std::string greeting;

    const std::string inArg  = "in=" ;
    const std::string outArg = "out=";
    const std::string errArg = "err=";

    FileStream f_input;
    FileStream f_output;
    FileStream f_err;
};
