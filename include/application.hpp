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

    Application(int argc, char* argv[])
        : vars(std::make_shared<Environment>(boost::this_process::environment()))
        , ios(std::make_shared<IOservice>(argc, argv))
//        , lastReturnCode(0)
        {
        }

    // Main loop of the program
    void run();

private:
    std::shared_ptr<Environment> vars;
    std::shared_ptr<IOservice> ios;
    Handler handler;
//    int lastReturnCode;
};

#endif //BASH_CLI_APPLICATION_HPP
