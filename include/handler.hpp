#pragma once

#include "cmd.hpp"
#include "extcmd.hpp"
#include "common.hpp"
#include "ioservice.hpp"

#include <unordered_map>
#include <string>
#include <memory>

/**
 * Class which handles all tokens after syntax analysis and executes commands with arguments from these tokens
 *      with given IO streams
 */
class Handler {
public:
    Handler(std::shared_ptr<IOservice> io)
            : exit(false)
            , ios(io)
    {}

    /**
     * Get flag if 'exit' command was given from user
     *
     * @return true if 'exit' was executed, else 0
     */
    bool isExit() const { return exit; }

    /**
     * Execute token on current Environment and given IO streams
     *
     * @return return code of the executed token
     */
    int exec(const token&, std::shared_ptr<Environment>, FileStream&, FileStream&);

private:
    bool exit;
    std::shared_ptr<IOservice> ios;
};
