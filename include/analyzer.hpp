#pragma once

#include "common.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <exception>


/**
 * Base class exception for syntax analysis errors
 */
struct SyntaxExc : public std::runtime_error {
    SyntaxExc(const std::string& str) : std::runtime_error(str) {}
};

/**
 * Lexer analysis exception
 */
struct LexerExc : public SyntaxExc {
    LexerExc(const std::string& str) : SyntaxExc(str) {}
};

/**
 * Parser analysis exceptions
 */
struct ParserExc : public SyntaxExc {
    ParserExc(const std::string& str) : SyntaxExc(str) {}
};


class Analyzer {
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

    enum class LexerState {
        Start = 0,
        SQ,
        DQ,
        BS,
        Dollar,
        Dollar_DQ,
        BS_DQ
    };

    static std::string buffer_extract(std::stringstream& ss) {
        std::string res = ss.str();
        ss.str("");
        return res;
    }

    std::string replace_var(const std::string& var) {
        if (var.empty()) {
            return "$";
        }

        if (env->find(var) != env->end()) {
            return env->get(var);
        }

        return "";
    }

public:
    Analyzer(std::shared_ptr<Environment> e) : env(e) {}

    /**
     * Start analysis
     *
     * @param input: string which will be analysed
     * @return vector of tokens
     */
    PipeLine process(const std::string& input) {
        return runParser(runLexer(input));
    }

    /**
     * Split input by pipes and replace variables by its` values.
     *
     * @param input: string with user`s input (command or piped commands).
     * @return vector of strings, each string is a separate command with it`s arguments.
     */
    std::vector<std::string> runLexer(const std::string& input) {
        std::vector<std::string> output;
        std::stringstream buffer;
        std::stringstream var;
        LexerState state = LexerState::Start;
        bool write_in_buff = true;
        bool inc = true;

        for (size_t i = 0; i < input.size(); i += inc) {
            char cur_ch = input[i];
            write_in_buff = true;
            inc = true;

            switch (state) {
                case LexerState::Start:
                    switch (cur_ch) {
                        case '|':
                            output.push_back(buffer_extract(buffer));
                            write_in_buff = false;
                            break;
                        case '\'':
                            state = LexerState::SQ;
                            break;
                        case '"':
                            state = LexerState::DQ;
                            break;
                        case '\\':
                            state = LexerState::BS;
                            break;
                        case '$':
                            write_in_buff = false;
                            state = LexerState::Dollar;
                            break;
                        default: {}
                    }
                    break;
                case LexerState::SQ:
                    if (cur_ch == '\'') {
                        state = LexerState::Start;
                    }
                    break;
                case LexerState::DQ:
                    if (cur_ch == '"') {
                        state = LexerState::Start;
                    } else if (cur_ch == '$') {
                        write_in_buff = false;
                        state = LexerState::Dollar_DQ;
                    } else if (cur_ch == '\\') {
                        state = LexerState::BS_DQ;
                    }
                    break;
                case LexerState::BS:
                    state = LexerState::Start;
                    break;
                case LexerState::Dollar:
                    write_in_buff = false;
                    if (std::isalpha(cur_ch) || std::isdigit(cur_ch) || cur_ch == '?') {
                        var << cur_ch;
                    } else {
                        // c_str() because on windows extra '\0' is added
                        buffer << replace_var(buffer_extract(var)).c_str();
                        inc = false;
                        state = LexerState::Start;
                    }
                    break;
                case LexerState::Dollar_DQ:
                    write_in_buff = false;
                    if (std::isalpha(cur_ch) || std::isdigit(cur_ch) || cur_ch == '?') {
                        var << cur_ch;
                    } else {
                        buffer << replace_var(buffer_extract(var)).c_str();
                        inc = false;
                        state = LexerState::DQ;
                    }
                    break;
                case LexerState::BS_DQ:
                    state = LexerState::DQ;
                    break;
                default: {}
            }

            if (write_in_buff) {
                buffer << cur_ch;
            }
        }

        if (state == LexerState::Dollar) {
            buffer << replace_var(buffer_extract(var)).c_str();
            state = LexerState::Start;
        }

        if (state == LexerState::Start) {
            output.push_back(buffer_extract(buffer));
        } else {
            throw LexerExc("Exception: wrong syntax!");
        }

        return output;
    }


    /**
    * Split commands into its` names and args.
    *
    * @param input: vector of strings, each string is a separate command.
    * @return vector of tokens, token.name - name of command,
    *                                 token.args - arguments of command.
    */
    PipeLine runParser(const std::vector<std::string>& input) {
        PipeLine output(input.size());

        for (size_t cmd_ind = 0; cmd_ind < input.size(); ++cmd_ind) {
            const std::string & cur_cmd = input[cmd_ind];
            token& cur_tok = output[cmd_ind];

            std::stringstream buffer;
            ParserState state = ParserState::Empty;
            bool is_first = true;
            bool inc = true;

            for (size_t i = 0; i < cur_cmd.size(); i += inc) {
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
                            cur_tok.add_word("=");
                            cur_tok.add_word(buffer_extract(buffer));
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
                            cur_tok.add_word(buffer_extract(buffer));
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
                    default: {}
                }
            }

            if (state == ParserState::RW || state == ParserState::VW) {
                cur_tok.add_word(buffer_extract(buffer));
                state = ParserState::Empty;
            }

            if (state != ParserState::Empty) {
                throw ParserExc("Exception: wrong syntax!");
            }
        }

        return output;
    }

private:
    std::shared_ptr<Environment> env;
};
