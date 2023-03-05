#ifndef BASH_CLI_APPLICATION_HPP
#define BASH_CLI_APPLICATION_HPP

#include "ioservice.hpp"
#include "handler.hpp"
#include "common.hpp"

#include <unordered_map>
#include <filesystem>

class Application {
public:
    //TODO: documentation

    Application(std::istream& is, std::ostream& os, std::ostream& es)
        : ios(is, os, es)
        , envState(std::filesystem::current_path(), es)
        , lastReturnCode(0)
        {}

    // Main loop of the program
    void run();

private:
    IOservice ios;
    Handler handler;
    EnvState envState;
    int lastReturnCode;
};

#endif //BASH_CLI_APPLICATION_HPP
