#ifndef BASH_CLI_HANDLER_HPP
#define BASH_CLI_HANDLER_HPP

#include "cmd.hpp"
#include "extcmd.hpp"
#include "common.hpp"
#include "ioservice.hpp"

#include <unordered_map>
#include <string>
#include <memory>

//TODO: documentation

//TODO: ref or not ref EnvState


class Handler {
public:
    Handler(std::shared_ptr<IOservice> ioserv);

    int exec(const job&, std::shared_ptr<Environment>, FILE*, FILE*);

private:
    std::shared_ptr<IOservice> ios;
    std::unordered_map<std::string, std::shared_ptr<Cmd>> commands;
    ExtCmd extCmd;
};


#endif //BASH_CLI_HANDLER_HPP
