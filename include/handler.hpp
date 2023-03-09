#ifndef BASH_CLI_HANDLER_HPP
#define BASH_CLI_HANDLER_HPP

#include "basecmd.hpp"
#include "common.hpp"

#include <unordered_map>
#include <string>
#include <memory>

//TODO: documentation

//TODO: ref or not ref EnvState


class Handler {
public:
    Handler();

    int handle(const job& j, EnvState& envState, boost::process::pstream& is, boost::process::pstream& os);

private:
    std::unordered_map<std::string, std::shared_ptr<BaseCmd>> commands;
};


#endif //BASH_CLI_HANDLER_HPP
