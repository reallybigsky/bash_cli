#ifndef BASH_CLI_HANDLER_HPP
#define BASH_CLI_HANDLER_HPP

#include "cmd.hpp"
#include "extcmd.hpp"
#include "common.hpp"
#include "ioservice.hpp"

#include <boost/process/system.hpp>

#include <unordered_map>
#include <string>
#include <memory>

//TODO: documentation

//TODO: ref or not ref EnvState


class Handler {
public:
    Handler();

    int exec(const job&, const EnvState&);

private:
    std::unordered_map<std::string, std::shared_ptr<Cmd>> commands;
    ExtCmd extCmd;
};


#endif //BASH_CLI_HANDLER_HPP
