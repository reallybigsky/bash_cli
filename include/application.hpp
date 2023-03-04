#ifndef BASH_CLI_APPLICATION_HPP
#define BASH_CLI_APPLICATION_HPP

#include "ioservice.hpp"
#include "handler.hpp"

#include <unordered_map>
#include <filesystem>

class Application {
public:
    //TODO: documentation

    Application(std::istream& is, std::ostream& os, std::ostream& es)
        : ios(is, os, es)
        , path(std::filesystem::current_path())
        , lastReturnCode(0)
        {}

    // Main loop of the program
    void run();

private:
    IOservice ios;
    Handler handler;
    std::unordered_map<std::string, std::string> varEnv;
    std::filesystem::path path;
    int lastReturnCode;
};

#endif //BASH_CLI_APPLICATION_HPP
