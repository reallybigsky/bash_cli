#pragma once

#include "ioservice.hpp"
#include "handler.hpp"
#include "analyzer.hpp"
#include "common.hpp"

#include <unordered_map>
#include <filesystem>
#include <span>

/**
 * Main class of the interpreter
 */
class Application {
public:
    Application(size_t argc, std::span<const char*> argv, std::string ios_greeting = "> ")
        : env(std::make_shared<Environment>())
        , ios(std::make_shared<IOService>(argc, argv, std::move(ios_greeting)))
        , handler(std::make_shared<Handler>(ios))
        , analyzer(std::make_shared<Analyzer>(env))
        , last_return_code(0)
    {
        env->vars.emplace("?", "-1");
    }

    /**
     * Main loop of the interpreter
     *
     * Loop ends if 'exit' command was executed, end of input file was reached or unknown exception was caught
     */
    void run();

private:

    /**
     * Executes given command pipeline
     */
    void exec_pipeline(const PipeLine&);

    std::shared_ptr<Environment> env;
    std::shared_ptr<IOService> ios;
    std::shared_ptr<Handler> handler;
    std::shared_ptr<Analyzer> analyzer;
    int last_return_code;
};
