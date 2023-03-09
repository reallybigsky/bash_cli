#include "application.hpp"
#include "preprocess.hpp"

#include <boost/process/system.hpp>

#include <string>
#include <sstream>

namespace bp = boost::process;

void Application::run() {
    //TODO: exceptions and errors
    //TODO: exit?
    while (true) {
        try {
            ios->greet();
            std::string user_input = ios->readLine();

            std::vector<std::string> after_lex = preprocess::runLexer(user_input);
            std::vector<job> after_parse = preprocess::runParser(after_lex);

            auto ps_in = std::make_shared<pstream>();
            for (size_t i = 0; i < after_parse.size(); ++i) {
                auto mask = static_cast<CmdPos>((i == 0 ? CmdPos::first : CmdPos::inner) +
                                                  (i == after_parse.size() - 1 ? CmdPos::last : CmdPos::inner));

                auto ps_out = std::make_shared<pstream>();

                EnvState envState = {vars, mask, ios, ps_in, ps_out};
                handler.exec(after_parse[i], envState);
                ps_in = ps_out;
            }
        } catch (...) {
            std::cout << "caught exception, aborting..." << std::endl;

            break;
        }
    }

}