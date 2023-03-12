#ifndef BASH_CLI_PREPROCESS_HPP
#define BASH_CLI_PREPROCESS_HPP

#include "common.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <exception>


struct SyntaxExc : public std::runtime_error {
    SyntaxExc(const std::string& str) : std::runtime_error(str) {}
};
struct LexerExc : public SyntaxExc {
    LexerExc(const std::string& str) : SyntaxExc(str) {}
};
struct ParserExc : public SyntaxExc {
    ParserExc(const std::string& str) : SyntaxExc(str) {}
};

class Parser {
private:
    enum class ParserState {
        Empty = 0,
        VW,
        RW,
        SQ,
        DQ,
        BS,
        BS_DQ
    };

    // TODO maby otimized
    static std::string buffer_extract(std::stringstream & ss) {
        std::string res = ss.str();
        ss.str(std::string());
        return res;
    }

    std::shared_ptr<Environment> env;

public:
    Parser(std::shared_ptr<Environment> e) : env(e) {}

    static PipeLine process(const std::string& input) {
        return runParser(runLexer(input));
    }

    /**
     * Split input by pipes and replace variables by its` values.
     * Currently assumes that there is no pipes and variables in command.
     *
     * @param `input` string with user`s input (command or piped commands).
     * @return `output` vector of strings, each string is a separate command with it`s arguments.
     */
    static std::vector<std::string> runLexer(const std::string& input) {
        std::vector<std::string> output;
        output.push_back(input);

        return output;
    }

    /**
    * Split commands into its` names and args.
    *
    * @param `input` vector of strings, each string is a separate command.
    * @return `output` vector of token, token.name - name of command,
    *                                 token.args - arguments of command.
    */
    static PipeLine runParser(const std::vector<std::string>& input) {
        PipeLine output(input.size());

        for (size_t cmd_ind = 0; cmd_ind < input.size(); ++cmd_ind) {
            const std::string & cur_cmd = input[cmd_ind];
            token& cur_job = output[cmd_ind];

            std::stringstream buffer;
            ParserState state = ParserState::Empty;
            bool is_first = true;
            bool inc = true;

            for (size_t i = 0; i < cur_cmd.size(); inc ? ++i : i ) {
                char cur_ch = cur_cmd[i];
                inc = true;

                switch (state) {
                    case ParserState::Empty:
                        if (is_first && (std::isalpha(cur_ch) || std::isdigit(cur_ch))) {
                            is_first = false;
                            state = ParserState::VW;
                            buffer << cur_ch;
                        } else if (!std::isspace(cur_ch)) {
                            is_first = false;
                            inc = false;
                            state = ParserState::RW;
                        }

                        break;
                    case ParserState::VW:
                        if (cur_ch == '=') {
                            cur_job.add_word("=");
                            cur_job.add_word(buffer_extract(buffer));
                            state = ParserState::RW;
                        } else if (std::isalpha(cur_ch) || std::isdigit(cur_ch)) {
                            buffer << cur_ch;
                        } else {
                            inc = false;
                            state = ParserState::RW;
                        }

                        break;
                    case ParserState::RW:
                        if (std::isspace(cur_ch)) {
                            cur_job.add_word(buffer_extract(buffer));
                            state = ParserState::Empty;
                        } else if (cur_ch == '\'') {
                            state = ParserState::SQ;
                        } else if (cur_ch == '"') {
                            state = ParserState::DQ;
                        } else if (cur_ch == '\\') {
                            state = ParserState::BS;
                        } else {
                            buffer << cur_ch;
                        }

                        break;
                    case ParserState::SQ:
                        if (cur_ch == '\'') {
                            state = ParserState::RW;
                        } else {
                            buffer << cur_ch;
                        }

                        break;
                    case ParserState::DQ:
                        if (cur_ch == '"') {
                            state = ParserState::RW;
                        } else if (cur_ch == '\\') {
                            state = ParserState::BS_DQ;
                        }
                        else {
                            buffer << cur_ch;
                        }

                        break;
                    case ParserState::BS:
                        buffer << cur_ch;
                        state = ParserState::RW;

                        break;
                    case ParserState::BS_DQ:
                        buffer << cur_ch;
                        state = ParserState::DQ;

                        break;
                }
            }

            if (state == ParserState::RW || state == ParserState::VW) {
                cur_job.add_word(buffer_extract(buffer));
                state = ParserState::Empty;
            }

            if (state != ParserState::Empty) {
                throw ParserExc("Exception: wrong syntax!");
            }
        }

        return output;
    }
};

#endif //BASH_CLI_PREPROCESS_HPP
