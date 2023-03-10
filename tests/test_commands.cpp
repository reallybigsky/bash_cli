#include <gtest/gtest.h>
#include "commands_utils.hpp"
#include "echo.hpp"
#include "pwd.hpp"
#include "cat.hpp"
#include "wc.hpp"


using namespace commands;

void clear_stringstream(std::stringstream& ss) {
    ss.str("");
    ss.clear();
}

TEST(TestEcho, echo) {
    job echo_job = {"echo", {"abc", "testtt", "someone"}};
    EnvState env(fs::current_path(), std::cout);
    Echo echo_cmd;


    std::stringstream result;
    std::string expected = "abc testtt someone";

    EXPECT_EQ(0, echo_cmd.run(echo_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());

    echo_job = {"echo", {}};

    clear_stringstream(result);
    expected = "";

    EXPECT_EQ(0, echo_cmd.run(echo_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());

}


TEST(TestPwd, pwd) {
    job pwd_job = {"pwd", {"abc", "someone"}};
    EnvState env(fs::current_path(), std::cout);
    Pwd pwd_cmd;

    std::stringstream result;
    std::string expected = "/home/avval/study/sem2/SD/bash_cli/src/tests/cmake-build-debug\n";
    EXPECT_EQ(0, pwd_cmd.run(pwd_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());

    pwd_job = {"pwd", {}};
    clear_stringstream(result);
    EXPECT_EQ(0, pwd_cmd.run(pwd_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());
}


TEST(TestCat, cat) {
    job cat_job = {"cat", {"test1.txt"}};
    EnvState env(fs::current_path(), std::cout);
    Cat cat_cmd;

    std::stringstream result;
    std::string expected = "some text\n"
                           "cat cat cat cat\n"
                           "dog dog dog\n"
                           "\n"
                           "or cat?\n";

    EXPECT_EQ(0, cat_cmd.run(cat_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());

    cat_job = {"cat", {"test1.txt", "test2.txt"}};
    clear_stringstream(result);
    expected = "some text\n"
               "cat cat cat cat\n"
               "dog dog dog\n"
               "\n"
               "or cat?\n"
               "Somebody once told me the world is gonna roll me\n"
               "I ain't the sharpest tool in the shed\n";

    EXPECT_EQ(0, cat_cmd.run(cat_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());

    cat_job = {"cat", {"test1.txt", "error"}};
    clear_stringstream(result);
    expected = "some text\n"
               "cat cat cat cat\n"
               "dog dog dog\n"
               "\n"
               "or cat?\n"
               "cat: error: No such file or directory\n";

    EXPECT_EQ(1, cat_cmd.run(cat_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());

    cat_job = {"cat", {}};
    expected = "No files were transferred";
    try {
        cat_cmd.run(cat_job, env, std::cin, result);
    } catch(const std::invalid_argument& err) {
        EXPECT_EQ(expected, err.what());
    } catch(...) {
        FAIL() << "expected std::invalid_argument";
    }

}


TEST(TestWc, wc) {
    job wc_job = {"wc", {"test1.txt"}};
    EnvState env(fs::current_path(), std::cout);
    Wc wc_cmd;

    std::stringstream result;
    std::string expected = " 5 11 47 test1.txt\n";

    EXPECT_EQ(0, wc_cmd.run(wc_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());

    wc_job = {"wc", {"test1.txt", "test2.txt"}};
    clear_stringstream(result);
    expected = "  5  11  47 test1.txt\n"
               "  2  18  87 test2.txt\n"
               "  7  29 134 total\n";

    EXPECT_EQ(0, wc_cmd.run(wc_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());

    wc_job = {"wc", {"test1.txt", "error"}};
    clear_stringstream(result);
    expected = " 5 11 47 test1.txt\n"
               "error: No such file or directory\n"
               " 5 11 47 total\n";

    EXPECT_EQ(1, wc_cmd.run(wc_job, env, std::cin, result));
    EXPECT_EQ(expected, result.str());

    wc_job = {"wc", {}};
    expected = "No files were transferred";
    try {
        wc_cmd.run(wc_job, env, std::cin, result);
    } catch(const std::invalid_argument& err) {
        EXPECT_EQ(expected, err.what());
    } catch(...) {
        FAIL() << "expected std::invalid_argument";
    }

}