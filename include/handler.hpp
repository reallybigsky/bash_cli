#ifndef BASH_CLI_HANDLER_HPP
#define BASH_CLI_HANDLER_HPP

#include "cmd.hpp"
#include "extcmd.hpp"
#include "common.hpp"
#include "ioservice.hpp"

#include <unordered_map>
#include <string>
#include <memory>


class Handler {
public:
    Handler(std::shared_ptr<IOservice> ioserv);

    bool isExit() const { return exit; }

    int exec(const token&, std::shared_ptr<Environment>, FILE*, FILE*);

private:
    bool exit;
    std::shared_ptr<IOservice> ios;
    std::unordered_map<std::string, std::shared_ptr<Cmd>> commands;
    commands::ExtCmd extCmd;
};


#endif //BASH_CLI_HANDLER_HPP
