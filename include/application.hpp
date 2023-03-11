#ifndef BASH_CLI_APPLICATION_HPP
#define BASH_CLI_APPLICATION_HPP

#include "ioservice.hpp"
#include "handler.hpp"
#include "common.hpp"

#include <unordered_map>
#include <filesystem>


class Application {
public:
    Application(int argc, char* argv[])
        : vars(std::make_shared<Environment>())
        , ios(std::make_shared<IOservice>(argc, argv))
        , handler(std::make_shared<Handler>(ios))
        , lastReturnCode(0)
    {
        vars->emplace("PWD", std::filesystem::current_path().string());
    }

    void run();

private:
    std::shared_ptr<Environment> vars;
    std::shared_ptr<IOservice> ios;
    std::shared_ptr<Handler> handler;
    int lastReturnCode;
};

#endif //BASH_CLI_APPLICATION_HPP
