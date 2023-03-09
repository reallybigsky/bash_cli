#ifndef BASH_CLI_PREPROCESS_HPP
#define BASH_CLI_PREPROCESS_HPP

#include "common.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <exception>

class Parser {
private:
    enum class parserStates {
        Empty = 0,
        VW,
        RW,
        SQ,
        DQ,
        BS,
        BS_DQ
    };


public:

private:

};

namespace preprocess {
  enum parserStates {Empty = 0, VW, RW, SQ, DQ, BS, BS_DQ}; // details
  std::string buffer_extract(std::stringstream & ss); // details

  /**
   * Exeptions for errors in preprocessing
   */
  struct parsingExeption : std::exception {};
  struct lexingExeption : std::exception {};

  /**
   * Break command by pipes and replace variables by it`s value.
   * Currently assumes what were is no pipes and variables in command.
   *
   * @param `input` string with user`s input (command or piped commands).
   * @return `output` vector of strings, each string is separate command.
   */
  std::vector<std::string> runLexer(const std::string& input) {
    std::vector<std::string> output;
    output.push_back(input);

    return output;
  } 

  /**
   * Break commands into it`s name and args.
   *
   * @param `input` vector of strings, each string is separate command.
   * @return `output` vector of job, job.name - name of command,
   *                                 job.args - arguments of command.
   */
  std::vector<job> runParser(const std::vector<std::string>& input) {
    std::vector<job> output(input.size());

    for (size_t cmd_ind = 0; cmd_ind < input.size(); ++cmd_ind) {
      const std::string & cur_cmd = input[cmd_ind];
      job & cur_job = output[cmd_ind];

      std::stringstream buffer;
      parserStates state = Empty;
      bool is_first = true;
      bool inc = true;

      for (size_t i = 0; i < cur_cmd.size(); inc ? ++i : i ) {
        char cur_ch = cur_cmd[i];
        inc = true;

        switch (state) {
          case Empty:
            if (is_first && (std::isalpha(cur_ch) || std::isdigit(cur_ch))) {
              is_first = false;
              state = VW;
              buffer << cur_ch;
            } else if (!std::isspace(cur_ch)) {
              is_first = false;
              inc = false;
              state = RW;
            }

            break;
          case VW:
            if (cur_ch == '=') {
              cur_job.add_word("=");
              cur_job.add_word(buffer_extract(buffer));
              state = RW;
            } else if (std::isalpha(cur_ch) || std::isdigit(cur_ch)) {
              buffer << cur_ch;
            } else {
              inc = false;
              state = RW;
            }

            break;
          case RW:
            if (std::isspace(cur_ch)) {
              cur_job.add_word(buffer_extract(buffer));
              state = Empty;
            } else if (cur_ch == '\'') {
              state = SQ;
            } else if (cur_ch == '"') {
              state = DQ;
            } else if (cur_ch == '\\') {
              state = BS;
            } else {
              buffer << cur_ch;
            }

            break;
          case SQ:
            if (cur_ch == '\'') {
              state = RW;
            } else {
              buffer << cur_ch;
            }

            break;
          case DQ:
            if (cur_ch == '"') {
              state = RW;
            } else if (cur_ch == '\\') {
              state = BS_DQ;
            }
            else {
              buffer << cur_ch;
            }

            break;
          case BS:
            buffer << cur_ch;
            state = RW;

            break;
          case BS_DQ:
            buffer << cur_ch;
            state = DQ;

            break;
        }
      }

      if (state == RW || state == VW) {
        cur_job.add_word(buffer_extract(buffer));
        state = Empty;
      }

      if (state != Empty) {
        throw parsingExeption();
      }
    }

    return output;
  }

  // TODO maby otimized
  std::string buffer_extract(std::stringstream & ss) {
    std::string res = ss.str();
    ss.str(std::string());
    return res;
  }
} // namespace preprocess

#endif //BASH_CLI_PREPROCESS_HPP
