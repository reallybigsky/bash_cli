#include "preprocess.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

void test_lexer_basic() {
  std::string input = "echo a";
  std::vector<std::string> lexer_output = preprocess::runLexer(input);

  assert(lexer_output[0] == input);
}

void test_parser_basic() {
  std::vector<std::string> input;
  std::vector<job> output;

  input.push_back("cmd");
  input.push_back("run_cmd");
  input.push_back("run_cmd arg1 arg2 arg3");
  input.push_back("ab\\ cd \\\" \\\' \\\\");

  output = preprocess::runParser(input);
  size_t i = 0;

  assert(output[i].name == "cmd");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "run_cmd");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "run_cmd");
  assert(output[i].args.size() == 3);
  assert(output[i].args[0] == "arg1");
  assert(output[i].args[1] == "arg2");
  assert(output[i].args[2] == "arg3");
  ++i;

  assert(output[i].name == "ab cd");
  assert(output[i].args.size() == 3);
  assert(output[i].args[0] == "\"");
  assert(output[i].args[1] == "\'");
  assert(output[i].args[2] == "\\");
  ++i;
}

void test_parser_double_quotes() {
  std::vector<std::string> input;
  std::vector<job> output;

  input.push_back("\"ab 12 $\"");
  input.push_back("\"ab 12 $\"12");
  input.push_back("\"ab\\ 12 $\"");
  input.push_back("\"ab\\\\ 12 $\"");
  input.push_back("\"ab\\\" 12 $\"");
  input.push_back("\"ab\"\"cd\"");
  input.push_back("\"ab\'\'cd\"");

  output = preprocess::runParser(input);
  size_t i = 0;

  assert(output[i].name == "ab 12 $");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab 12 $12");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab 12 $");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab\\ 12 $");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab\" 12 $");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "abcd");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab\'\'cd");
  assert(output[i].args.size() == 0);
  ++i;
}

void test_parser_single_quotes() {
  std::vector<std::string> input;
  std::vector<job> output;

  input.push_back("\'ab 12 $\'");
  input.push_back("\'ab 12 $\'12");
  input.push_back("\'ab\\ 12 $\'");
  input.push_back("\'ab\\\\ 12 $\'");
  input.push_back("\'ab\\\" 12 $\'");
  input.push_back("\'ab\'\'cd\'");
  input.push_back("\'ab\"\"cd\'");

  output = preprocess::runParser(input);
  size_t i = 0;

  assert(output[i].name == "ab 12 $");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab 12 $12");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab\\ 12 $");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab\\\\ 12 $");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab\\\" 12 $");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "abcd");
  assert(output[i].args.size() == 0);
  ++i;

  assert(output[i].name == "ab\"\"cd");
  assert(output[i].args.size() == 0);
  ++i;
}

void test_parser_assignment() {
  std::vector<std::string> input;
  std::vector<job> output;

  input.push_back("abc123= 123");
  input.push_back("1a2b3c=");
  input.push_back("a=abc 123");
  input.push_back("a$=abc");

  output = preprocess::runParser(input);
  size_t i = 0;

  assert(output[i].name == "=");
  assert(output[i].args[0] == "abc123");
  assert(output[i].args[1] == " 123");
  ++i;

  assert(output[i].name == "=");
  assert(output[i].args[0] == "1a2b3c");
  assert(output[i].args[1] == "");
  ++i;

  assert(output[i].name == "=");
  assert(output[i].args[0] == "a");
  assert(output[i].args[1] == "abc 123");
  ++i;

  assert(output[i].name == "a$=abc");
  assert(output[i].args.size() == 0);
  ++i;
}

void test_parser_all() {
  std::vector<std::string> input;
  std::vector<job> output;

  input.push_back("cmd a=b \"f\"\'s\'");
  input.push_back("a\"bc\"=d f\\\"s");

  output = preprocess::runParser(input);
  size_t i = 0;

  assert(output[i].name == "cmd");
  assert(output[i].args.size() == 2);
  assert(output[i].args[0] == "a=b");
  assert(output[i].args[1] == "fs");
  ++i;

  assert(output[i].name == "abc=d");
  assert(output[i].args.size() == 1);
  assert(output[i].args[0] == "f\"s");
  ++i;
}

// TODO change to error handl
void test_parser_error() {
  std::vector<std::string> input;
  std::vector<job> output;

  input = std::vector<std::string>();
  input.push_back("cmd\' a=b");
  bool is_exeption = false;
  try {
    output = preprocess::runParser(input);
  } catch (const preprocess::parsingExeption & e) {
    is_exeption = true;
  }
  assert(is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd \' a=b\'");
  is_exeption = false;
  try {
    output = preprocess::runParser(input);
  } catch (const preprocess::parsingExeption & e) {
    is_exeption = true;
  }
  assert(!is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd \" a=b\'");
  is_exeption = false;
  try {
    output = preprocess::runParser(input);
  } catch (const preprocess::parsingExeption & e) {
    is_exeption = true;
  }
  assert(is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd \" a=b\"");
  is_exeption = false;
  try {
    output = preprocess::runParser(input);
  } catch (const preprocess::parsingExeption & e) {
    is_exeption = true;
  }
  assert(!is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd adf d\\");
  is_exeption = false;
  try {
    output = preprocess::runParser(input);
  } catch (const preprocess::parsingExeption & e) {
    is_exeption = true;
  }
  assert(is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd \\ ");
  is_exeption = false;
  try {
    output = preprocess::runParser(input);
  } catch (const preprocess::parsingExeption & e) {
    is_exeption = true;
  }
  assert(!is_exeption);

  input = std::vector<std::string>();
  input.push_back("cm\"\\");
  is_exeption = false;
  try {
    output = preprocess::runParser(input);
  } catch (const preprocess::parsingExeption & e) {
    is_exeption = true;
  }
  assert(is_exeption);
}

int main(int argc, char* argv[]) {
  test_lexer_basic();

  test_parser_basic();
  test_parser_double_quotes();
  test_parser_single_quotes();
  test_parser_assignment();
  test_parser_all();

  test_parser_error();

  return 0;
};
