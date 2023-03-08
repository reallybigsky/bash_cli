#ifndef BASH_CLI_PREPROCESS_HPP
#define BASH_CLI_PREPROCESS_HPP

#include "common.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

namespace preprocess {
  enum parserStates {Empty = 0, VW, RW, SQ, DQ, BS, BS_DQ}; // details
  bool is_space(char ch); // details
  bool is_var(char ch); // details
  std::string buffer_extract(std::stringstream & ss); // details

  struct parsingExeption : std::exception {};
  struct lexingExeption : std::exception {};

  // without pipe and replace
  std::vector<std::string> runLexer(const std::string& input) {
    std::vector<std::string> output;
    output.push_back(input);

    return output;
  } 

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
            if (is_first && is_var(cur_ch)) {
              is_first = false;
              state = VW;
              buffer << cur_ch;
            } else if (!is_space(cur_ch)) {
              is_first = false;
              inc = false;
              state = RW;
            }

            break;
          case VW:
            if (cur_ch == '=') {
              cur_job.name = '=';
              cur_job.args.push_back(buffer_extract(buffer));
              cur_job.args.push_back(cur_cmd.substr(i+1));
              i = cur_cmd.size();
              state = Empty;
            } else if (is_var(cur_ch)) {
              buffer << cur_ch;
            } else {
              inc = false;
              state = RW;
            }

            break;
          case RW:
            if (is_space(cur_ch)) {
              if (cur_job.name.empty()) {
                cur_job.name = buffer_extract(buffer);
              } else {
                cur_job.args.push_back(buffer_extract(buffer));
              }

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
        // TODO remove code repetition
        if (cur_job.name.empty()) {
          cur_job.name = buffer_extract(buffer);
        } else {
          cur_job.args.push_back(buffer_extract(buffer));
        }

        state = Empty;
      }

      if (state != Empty) {
        throw parsingExeption(); // TODO change to error handle
      }
    }

    return output;
  }


  bool is_space(char ch) {
    return ch == ' '; // TODO other space symbols
  }

  bool is_var(char ch) {
    if (ch >= 'a' && ch <= 'z' ||
        ch >= 'A' && ch <= 'Z' ||
        ch >= '1' && ch <= '9') 
    {
      return true;
    }

    return false;
  }

  // TODO maby otimized
  std::string buffer_extract(std::stringstream & ss) {
    std::string res = ss.str();
    ss.str(std::string());
    return res;
  }
} // namespace preprocess

#endif //BASH_CLI_PREPROCESS_HPP
