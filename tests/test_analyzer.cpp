#include "analyzer.hpp"

#include <string>
#include <vector>
#include <cassert>

std::shared_ptr<Environment> env(std::make_shared<Environment>());
Analyzer parser(env);

void test_lexer_basic() {
  std::string input;
  std::vector<std::string> output;

  env->clear();
  input = "pwd";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  input = "echo a";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);
}

void test_lexer_pipe() {
  std::string input;
  std::vector<std::string> output;

  env->clear();
  input = "pwd | echo a";
  output = parser.runLexer(input);
  assert(output.size() == 2);
  assert(output[0] == "pwd ");
  assert(output[1] == " echo a");

  env->clear();
  input = "abc a b c | echo | pwd | wc";
  output = parser.runLexer(input);
  assert(output.size() == 4);
  assert(output[0] == "abc a b c ");
  assert(output[1] == " echo ");
  assert(output[2] == " pwd ");
  assert(output[3] == " wc");
}

void test_lexer_replace() {
  std::string input;
  std::vector<std::string> output;

  env->clear();
  (*env)["a"] = "123";
  input = "$";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == "$");

  env->clear();
  (*env)["a"] = "123";
  input = "$a";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == "123");

  env->clear();
  input = "$a";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == "");

  env->clear();
  (*env)["a"] = "ec";
  (*env)["b"] = "ho";
  input = "$a$b";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == "echo");

  env->clear();
  (*env)["a"] = "|";
  (*env)["b"] = "echo";
  input = "$b $a $b $a $b";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == "echo | echo | echo");
}

void test_lexer_single_quotes() {
  std::string input;
  std::vector<std::string> output;

  env->clear();
  (*env)["a"] = "123";
  input = "\'\"\'";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  input = "\'a\'";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  input = "\'$a\'";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  (*env)["b"] = "cde";
  (*env)["a"] = "123";
  input = "\'$a | $b | $var\'";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  input = "\'|\'|\'$a\'";
  output = parser.runLexer(input);
  assert(output.size() == 2);
  assert(output[0] == "\'|\'");
  assert(output[1] == "\'$a\'");
}

void test_lexer_double_quotes() {
  std::string input;
  std::vector<std::string> output;

  env->clear();
  input = "\"\'\"";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  input = "\"a\"";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  input = "\"$a\"";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == "\"123\"");

  env->clear();
  (*env)["a"] = "123";
  (*env)["b"] = "cde";
  (*env)["var"] = "abc dba";
  input = "\"$a | $b | $var\"";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == "\"123 | cde | abc dba\"");

  env->clear();
  (*env)["a"] = "123";
  input = "\"|\"|\"$a\"";
  output = parser.runLexer(input);
  assert(output.size() == 2);
  assert(output[0] == "\"|\"");
  assert(output[1] == "\"123\"");
}

void test_lexer_bs() {
  std::string input;
  std::vector<std::string> output;

  env->clear();
  (*env)["a"] = "123";
  input = "\\$a";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  input = "\"\\$a\"";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  input = "abc \\\" db ";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  input = "\"abc \\\" db \"";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);

  env->clear();
  (*env)["a"] = "123";
  input = "abc \\| bcd";
  output = parser.runLexer(input);
  assert(output.size() == 1);
  assert(output[0] == input);
}

void test_lexer_all() {
  std::string input;
  std::vector<std::string> output;

  env->clear();
  (*env)["a"] = "123";
  (*env)["var"] = "echo abc";
  input = "$var|echo \"$a\" \\| | \'pwd $var\' ||";
  output = parser.runLexer(input);
  assert(output.size() == 5);
  assert(output[0] == "echo abc");
  assert(output[1] == "echo \"123\" \\| ");
  assert(output[2] == " \'pwd $var\' ");
  assert(output[3] == "");
  assert(output[4] == "");
}

void test_lexer_error() {
  std::string input;
  std::vector<std::string> output;
  bool is_exeption;

  env->clear();
  input = "abc \\";
  is_exeption = false;
  try {
    output = parser.runLexer(input);
  } catch (const LexerExc & e) {
    is_exeption = true;
  }
  assert(is_exeption);

  env->clear();
  input = "abc | \" aasdf";
  is_exeption = false;
  try {
    output = parser.runLexer(input);
  } catch (const LexerExc & e) {
    is_exeption = true;
  }
  assert(is_exeption);

  env->clear();
  input = "abc | \' asf | \"";
  is_exeption = false;
  try {
    output = parser.runLexer(input);
  } catch (const LexerExc & e) {
    is_exeption = true;
  }
  assert(is_exeption);

  env->clear();
  input = "abc | \' asf\\\' | \'";
  is_exeption = false;
  try {
    output = parser.runLexer(input);
  } catch (const LexerExc & e) {
    is_exeption = true;
  }
  assert(is_exeption);
}

void test_parser_basic() {
  env->clear();
  std::vector<std::string> input;
  std::vector<token> output;

  input.push_back("cmd");
  input.push_back("run_cmd");
  input.push_back("run_cmd arg1 arg2 arg3");
  input.push_back("ab\\ cd \\\" \\\' \\\\");

  output = parser.runParser(input);
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
  env->clear();
  std::vector<std::string> input;
  std::vector<token> output;

  input.push_back("\"ab 12 $\"");
  input.push_back("\"ab 12 $\"12");
  input.push_back("\"ab\\ 12 $\"");
  input.push_back("\"ab\\\\ 12 $\"");
  input.push_back("\"ab\\\" 12 $\"");
  input.push_back("\"ab\"\"cd\"");
  input.push_back("\"ab\'\'cd\"");

  output = parser.runParser(input);
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
  env->clear();
  std::vector<std::string> input;
  std::vector<token> output;

  input.push_back("\'ab 12 $\'");
  input.push_back("\'ab 12 $\'12");
  input.push_back("\'ab\\ 12 $\'");
  input.push_back("\'ab\\\\ 12 $\'");
  input.push_back("\'ab\\\" 12 $\'");
  input.push_back("\'ab\'\'cd\'");
  input.push_back("\'ab\"\"cd\'");

  output = parser.runParser(input);
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
  env->clear();
  std::vector<std::string> input;
  std::vector<token> output;

  input.push_back("abc123=123");
  input.push_back("1a2b3c=");
  input.push_back("1a2b3c= 12 bc");
  input.push_back("a=abc 123");
  input.push_back("a$=abc");

  output = parser.runParser(input);
  size_t i = 0;

  assert(output[i].name == "=");
  assert(output[i].args[0] == "abc123");
  assert(output[i].args[1] == "123");
  ++i;

  assert(output[i].name == "=");
  assert(output[i].args[0] == "1a2b3c");
  assert(output[i].args[1] == "");
  ++i;

  assert(output[i].name == "=");
  assert(output[i].args[0] == "1a2b3c");
  assert(output[i].args[1] == "");
  assert(output[i].args[2] == "12");
  assert(output[i].args[3] == "bc");
  ++i;

  assert(output[i].name == "=");
  assert(output[i].args[0] == "a");
  assert(output[i].args[1] == "abc");
  assert(output[i].args[2] == "123");
  ++i;

  assert(output[i].name == "a$=abc");
  assert(output[i].args.size() == 0);
  ++i;
}

void test_parser_all() {
  env->clear();
  std::vector<std::string> input;
  std::vector<token> output;

  input.push_back("cmd a=b \"f\"\'s\'");
  input.push_back("a\"bc\"=d f\\\"s");
  input.push_back("var=so\" d \" f\\\"s");

  output = parser.runParser(input);
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

  assert(output[i].name == "=");
  assert(output[i].args.size() == 3);
  assert(output[i].args[0] == "var");
  assert(output[i].args[1] == "so d ");
  assert(output[i].args[2] == "f\"s");
  ++i;
}

void test_parser_error() {
  env->clear();
  std::vector<std::string> input;
  std::vector<token> output;

  input = std::vector<std::string>();
  input.push_back("cmd\' a=b");
  bool is_exeption = false;
  try {
    output = parser.runParser(input);
  } catch (const ParserExc & e) {
    is_exeption = true;
  }
  assert(is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd \' a=b\'");
  is_exeption = false;
  try {
    output = parser.runParser(input);
  } catch (const ParserExc & e) {
    is_exeption = true;
  }
  assert(!is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd \" a=b\'");
  is_exeption = false;
  try {
    output = parser.runParser(input);
  } catch (const ParserExc & e) {
    is_exeption = true;
  }
  assert(is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd \" a=b\"");
  is_exeption = false;
  try {
    output = parser.runParser(input);
  } catch (const ParserExc & e) {
    is_exeption = true;
  }
  assert(!is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd adf d\\");
  is_exeption = false;
  try {
    output = parser.runParser(input);
  } catch (const ParserExc & e) {
    is_exeption = true;
  }
  assert(is_exeption);

  input = std::vector<std::string>();
  input.push_back("cmd \\ ");
  is_exeption = false;
  try {
    output = parser.runParser(input);
  } catch (const ParserExc & e) {
    is_exeption = true;
  }
  assert(!is_exeption);

  input = std::vector<std::string>();
  input.push_back("cm\"\\");
  is_exeption = false;
  try {
    output = parser.runParser(input);
  } catch (const ParserExc & e) {
    is_exeption = true;
  }
  assert(is_exeption);
}

void test_all() {
  std::string input;
  PipeLine output;

  env->clear();
  (*env)["var"] = "echo a b c";
  (*env)["a"] = "123";
  (*env)["b"] = "var=cda";
  input = "$var| echo $a | pwd \\$var | wc $ \\\' | $b \"12  23\" | exit \'$a  $b\'";
  output = parser.process(input);
  
  assert(output.size() == 6);

  assert(output[0].name == "echo");
  assert(output[0].args[0] == "a");
  assert(output[0].args[1] == "b");
  assert(output[0].args[2] == "c");

  assert(output[1].name == "echo");
  assert(output[1].args[0] == "123");
  
  assert(output[2].name == "pwd");
  assert(output[2].args[0] == "$var");

  assert(output[3].name == "wc");
  assert(output[3].args[0] == "$");
  assert(output[3].args[1] == "\'");

  assert(output[4].name == "=");
  assert(output[4].args[0] == "var");
  assert(output[4].args[1] == "cda");
  assert(output[4].args[2] == "12  23");

  assert(output[5].name == "exit");
  assert(output[5].args[0] == "$a  $b");
}

int main(int argc, char* argv[]) {
  test_lexer_basic();
  test_lexer_pipe();
  test_lexer_replace();
  test_lexer_single_quotes();
  test_lexer_double_quotes();
  test_lexer_bs();
  test_lexer_all();
  test_lexer_error();

  test_parser_basic();
  test_parser_double_quotes();
  test_parser_single_quotes();
  test_parser_assignment();
  test_parser_all();
  test_parser_error();

  test_all();

  return 0;
};
