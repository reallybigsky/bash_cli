#include "analyzer.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <iostream>

std::shared_ptr<Environment> env(std::make_shared<Environment>());
Analyzer parser(env);

TEST(TestLexerBasic, lexer_basic) {
    std::string input;
    std::vector<std::string> output;

    env->vars.clear();
    input = "pwd";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    input = "echo a";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);
}

TEST(TestLexerPipe, lexer_pipe) {
    std::string input;
    std::vector<std::string> output;

    env->vars.clear();
    input = "pwd | echo a";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 2);
    EXPECT_EQ(output[0], "pwd ");
    EXPECT_EQ(output[1], " echo a");

    env->vars.clear();
    input = "abc a b c | echo | pwd | wc";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 4);
    EXPECT_EQ(output[0], "abc a b c ");
    EXPECT_EQ(output[1], " echo ");
    EXPECT_EQ(output[2], " pwd ");
    EXPECT_EQ(output[3], " wc");
}

TEST(TestLexerReplace, lexer_replace) {
    std::string input;
    std::vector<std::string> output;

    env->vars.clear();
    env->vars["a"] = "123";
    input = "$";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], "$");

    env->vars.clear();
    env->vars["a"] = "123";
    input = "$a";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], "123");

    env->vars.clear();
    input = "$a";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], "");

    env->vars.clear();
    env->vars["a"] = "ec";
    env->vars["b"] = "ho";
    input = "$a$b";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], "echo");

    env->vars.clear();
    env->vars["a"] = "|";
    env->vars["b"] = "echo";
    input = "$b $a $b $a $b";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], "echo | echo | echo");
}

TEST(TestLexerSingleQuotes, lexer_single_quotes) {
    std::string input;
    std::vector<std::string> output;

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\'\"\'";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\'a\'";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\'$a\'";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    env->vars["b"] = "cde";
    env->vars["a"] = "123";
    input = "\'$a | $b | $var\'";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\'|\'|\'$a\'";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size() ,2);
    EXPECT_EQ(output[0], "\'|\'");
    EXPECT_EQ(output[1], "\'$a\'");
}

TEST(TestLexerDoubleQuotes, lexer_double_quotes) {
    std::string input;
    std::vector<std::string> output;

    env->vars.clear();
    input = "\"\'\"";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\"a\"";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\"$a\"";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], "\"123\"");

    env->vars.clear();
    env->vars["a"] = "123";
    env->vars["b"] = "cde";
    env->vars["var"] = "abc dba";
    input = "\"$a | $b | $var\"";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], "\"123 | cde | abc dba\"");

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\"|\"|\"$a\"";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size() ,2);
    EXPECT_EQ(output[0], "\"|\"");
    EXPECT_EQ(output[1], "\"123\"");
}

TEST(TestLexerBS, lexer_bs) {
    std::string input;
    std::vector<std::string> output;

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\\$a";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\"\\$a\"";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    input = "abc \\\" db ";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    input = "\"abc \\\" db \"";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);

    env->vars.clear();
    env->vars["a"] = "123";
    input = "abc \\| bcd";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], input);
}

TEST(TestLexerAll, lexer_lexer_all) {
    std::string input;
    std::vector<std::string> output;

    env->vars.clear();
    env->vars["a"] = "123";
    env->vars["var"] = "echo abc";
    input = "$var|echo \"$a\" \\| | \'pwd $var\' ||";
    output = parser.run_lexer(input);
    EXPECT_EQ(output.size(), 5);
    EXPECT_EQ(output[0], "echo abc");
    EXPECT_EQ(output[1], "echo \"123\" \\| ");
    EXPECT_EQ(output[2], " \'pwd $var\' ");
    EXPECT_EQ(output[3], "");
    EXPECT_EQ(output[4], "");
}

TEST(TestLexerError, lexer_error) {
    std::string input;
    std::vector<std::string> output;
    bool is_exception;

    env->vars.clear();
    input = "abc \\";
    is_exception = false;
    try {
        output = parser.run_lexer(input);
    } catch (const LexerExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(is_exception);

    env->vars.clear();
    input = "abc | \" aasdf";
    is_exception = false;
    try {
        output = parser.run_lexer(input);
    } catch (const LexerExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(is_exception);

    env->vars.clear();
    input = "abc | \' asf | \"";
    is_exception = false;
    try {
        output = parser.run_lexer(input);
    } catch (const LexerExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(is_exception);

    env->vars.clear();
    input = "abc | \' asf\\\' | \'";
    is_exception = false;
    try {
        output = parser.run_lexer(input);
    } catch (const LexerExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(is_exception);
}

TEST(TestParserBasic, parser_basic) {
    env->vars.clear();
    std::vector<std::string> input;
    std::vector<CmdToken> output;

    input.emplace_back("cmd");
    input.emplace_back("run_cmd");
    input.emplace_back("run_cmd arg1 arg2 arg3");
    input.emplace_back("ab\\ cd \\\" \\\' \\\\");

    output = parser.run_parser(input);
    size_t i = 0;

    EXPECT_EQ(output[i].name, "cmd");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "run_cmd");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "run_cmd");
    EXPECT_EQ(output[i].args.size(), 3);
    EXPECT_EQ(output[i].args[0], "arg1");
    EXPECT_EQ(output[i].args[1], "arg2");
    EXPECT_EQ(output[i].args[2], "arg3");
    ++i;

    EXPECT_EQ(output[i].name, "ab cd");
    EXPECT_EQ(output[i].args.size(), 3);
    EXPECT_EQ(output[i].args[0], "\"");
    EXPECT_EQ(output[i].args[1], "\'");
    EXPECT_EQ(output[i].args[2], "\\");
    ++i;
}

TEST(TestParserDoubleQuotes, parser_double_quotes) {
    env->vars.clear();
    std::vector<std::string> input;
    std::vector<CmdToken> output;

    input.emplace_back("\"ab 12 $\"");
    input.emplace_back("\"ab 12 $\"12");
    input.emplace_back("\"ab\\ 12 $\"");
    input.emplace_back("\"ab\\\\ 12 $\"");
    input.emplace_back("\"ab\\\" 12 $\"");
    input.emplace_back("\"ab\"\"cd\"");
    input.emplace_back("\"ab\'\'cd\"");

    output = parser.run_parser(input);
    size_t i = 0;

    EXPECT_EQ(output[i].name, "ab 12 $");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab 12 $12");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab 12 $");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab\\ 12 $");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab\" 12 $");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "abcd");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab\'\'cd");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;
}

TEST(TestParserSingleQutes, parser_single_quotes) {
    env->vars.clear();
    std::vector<std::string> input;
    std::vector<CmdToken> output;

    input.emplace_back("\'ab 12 $\'");
    input.emplace_back("\'ab 12 $\'12");
    input.emplace_back("\'ab\\ 12 $\'");
    input.emplace_back("\'ab\\\\ 12 $\'");
    input.emplace_back("\'ab\\\" 12 $\'");
    input.emplace_back("\'ab\'\'cd\'");
    input.emplace_back("\'ab\"\"cd\'");

    output = parser.run_parser(input);
    size_t i = 0;

    EXPECT_EQ(output[i].name, "ab 12 $");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab 12 $12");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab\\ 12 $");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab\\\\ 12 $");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab\\\" 12 $");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "abcd");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;

    EXPECT_EQ(output[i].name, "ab\"\"cd");
    EXPECT_EQ(output[i].args.size(), 0);
    ++i;
}

TEST(TestParserAssigment, parser_assignment) {
    env->vars.clear();
    std::vector<std::string> input;
    std::vector<CmdToken> output;

    input.emplace_back("abc123=123");
    input.emplace_back("1a2b3c=");
    input.emplace_back("1a2b3c= 12 bc");
    input.emplace_back("a=abc 123");
    input.emplace_back("a$=abc");

    output = parser.run_parser(input);
    size_t i = 0;

    EXPECT_EQ(output[i].name ,"=");
    EXPECT_EQ(output[i].args[0] ,"abc123");
    EXPECT_EQ(output[i].args[1] ,"123");
    ++i;

    EXPECT_EQ(output[i].name ,"=");
    EXPECT_EQ(output[i].args[0] ,"1a2b3c");
    EXPECT_EQ(output[i].args[1] ,"");
    ++i;

    EXPECT_EQ(output[i].name ,"=");
    EXPECT_EQ(output[i].args[0] ,"1a2b3c");
    EXPECT_EQ(output[i].args[1] ,"");
    EXPECT_EQ(output[i].args[2] ,"12");
    EXPECT_EQ(output[i].args[3] ,"bc");
    ++i;

    EXPECT_EQ(output[i].name ,"=");
    EXPECT_EQ(output[i].args[0] ,"a");
    EXPECT_EQ(output[i].args[1] ,"abc");
    EXPECT_EQ(output[i].args[2] ,"123");
    ++i;

    EXPECT_EQ(output[i].name ,"a$=abc");
    EXPECT_EQ(output[i].args.size() ,0);
    ++i;
}

TEST(TestParserAll, parser_all) {
    env->vars.clear();
    std::vector<std::string> input;
    std::vector<CmdToken> output;

    input.emplace_back("cmd a=b \"f\"\'s\'");
    input.emplace_back("a\"bc\"=d f\\\"s");
    input.emplace_back("var=so\" d \" f\\\"s");

    output = parser.run_parser(input);
    size_t i = 0;

    EXPECT_EQ(output[i].name ,"cmd");
    EXPECT_EQ(output[i].args.size() ,2);
    EXPECT_EQ(output[i].args[0] ,"a=b");
    EXPECT_EQ(output[i].args[1] ,"fs");
    ++i;

    EXPECT_EQ(output[i].name ,"abc=d");
    EXPECT_EQ(output[i].args.size() ,1);
    EXPECT_EQ(output[i].args[0] ,"f\"s");
    ++i;

    EXPECT_EQ(output[i].name ,"=");
    EXPECT_EQ(output[i].args.size() ,3);
    EXPECT_EQ(output[i].args[0] ,"var");
    EXPECT_EQ(output[i].args[1] ,"so d ");
    EXPECT_EQ(output[i].args[2] ,"f\"s");
    ++i;
}

TEST(TestParserError, parser_error) {
    env->vars.clear();
    std::vector<std::string> input;
    std::vector<CmdToken> output;

    input = std::vector<std::string>();
    input.emplace_back("cmd\' a=b");
    bool is_exception = false;
    try {
        output = parser.run_parser(input);
    } catch (const ParserExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(is_exception);

    input = std::vector<std::string>();
    input.emplace_back("cmd \' a=b\'");
    is_exception = false;
    try {
        output = parser.run_parser(input);
    } catch (const ParserExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(!is_exception);

    input = std::vector<std::string>();
    input.emplace_back("cmd \" a=b\'");
    is_exception = false;
    try {
        output = parser.run_parser(input);
    } catch (const ParserExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(is_exception);

    input = std::vector<std::string>();
    input.emplace_back("cmd \" a=b\"");
    is_exception = false;
    try {
        output = parser.run_parser(input);
    } catch (const ParserExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(!is_exception);

    input = std::vector<std::string>();
    input.emplace_back("cmd adf d\\");
    is_exception = false;
    try {
        output = parser.run_parser(input);
    } catch (const ParserExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(is_exception);

    input = std::vector<std::string>();
    input.emplace_back("cmd \\ ");
    is_exception = false;
    try {
        output = parser.run_parser(input);
    } catch (const ParserExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(!is_exception);

    input = std::vector<std::string>();
    input.emplace_back("cm\"\\");
    is_exception = false;
    try {
        output = parser.run_parser(input);
    } catch (const ParserExc & e) {
        is_exception = true;
    }
    EXPECT_TRUE(is_exception);
}

TEST(TestAll, all) {
    std::cout << "Analyzer::Lexer + Analyzer::Parser..." << std::endl;
    std::string input;
    PipeLine output;

    env->vars.clear();
    env->vars["var"] = "echo a b c";
    env->vars["a"] = "123";
    env->vars["b"] = "var=cda";
    input = "$var| echo $a | pwd \\$var | wc $ \\\' | $b \"12  23\" | exit \'$a  $b\'";
    output = parser.process(input);

    EXPECT_EQ(output.size() ,6);

    EXPECT_EQ(output[0].name ,"echo");
    EXPECT_EQ(output[0].args[0] ,"a");
    EXPECT_EQ(output[0].args[1] ,"b");
    EXPECT_EQ(output[0].args[2] ,"c");

    EXPECT_EQ(output[1].name ,"echo");
    EXPECT_EQ(output[1].args[0] ,"123");

    EXPECT_EQ(output[2].name ,"pwd");
    EXPECT_EQ(output[2].args[0] ,"$var");

    EXPECT_EQ(output[3].name ,"wc");
    EXPECT_EQ(output[3].args[0] ,"$");
    EXPECT_EQ(output[3].args[1] ,"\'");

    EXPECT_EQ(output[4].name ,"=");
    EXPECT_EQ(output[4].args[0] ,"var");
    EXPECT_EQ(output[4].args[1] ,"cda");
    EXPECT_EQ(output[4].args[2] ,"12  23");

    EXPECT_EQ(output[5].name ,"exit");
    EXPECT_EQ(output[5].args[0] ,"$a  $b");
    std::cout << "Analyzer::Lexer + Analyzer::Parser OK!" << std::endl;
}
