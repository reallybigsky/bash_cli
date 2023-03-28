#ifndef BASH_CLI_APPLICATION_HPP
#define BASH_CLI_APPLICATION_HPP

#include "ioservice.hpp"
#include "handler.hpp"
#include "analyzer.hpp"
#include "common.hpp"

#include <unordered_map>
#include <filesystem>

/**
 * Main class of the interpreter
 */
class Application {
public:
    Application(int argc, const char* argv[])
        : env(std::make_shared<Environment>())
        , ios(std::make_shared<IOservice>(argc, argv))
        , handler(std::make_shared<Handler>(ios))
        , analyzer(std::make_shared<Analyzer>(env))
        , lastReturnCode(0)
    {
        env->emplace("PWD", std::filesystem::current_path().string());
        env->emplace("?", "-1");
    }

    /**
     * Main loop of the interpreter
     *
     * Loop ends if 'exit' command was executed, end of input file was reached or unknown exception was caught
     */
    void run();

private:
    std::shared_ptr<Environment> env;
    std::shared_ptr<IOservice> ios;
    std::shared_ptr<Handler> handler;
    std::shared_ptr<Analyzer> analyzer;
    int lastReturnCode;
};

#endif //BASH_CLI_APPLICATION_HPP
