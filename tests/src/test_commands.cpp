#include <gtest/gtest.h>
#include "file_utils.hpp"
#include "echo.hpp"
#include "pwd.hpp"
#include "cat.hpp"
#include "wc.hpp"


using namespace commands;

std::string read_file_to_string(FILE* out){
    rewind(out);
    std::stringstream result;
    while (auto line = FileUtils::readLine(out))
        result << line.value();

    rewind(out);

    return result.str();
}

TEST(TestEcho, echo) {
    FILE* i_file = nullptr, *o_file = nullptr, *e_file = tmpfile();
    token echo_job = {"echo", {"abc", "testtt", "someone"}};
    auto env = std::make_shared<Environment>();
    env->emplace("PWD", std::filesystem::current_path().string());
    Echo echo_cmd;
    o_file = tmpfile();

    std::string expected = "abc testtt someone\n";
    EXPECT_EQ(0, echo_cmd.run(echo_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    echo_job = {"echo", {}};
    expected = "\n";
    fclose(o_file);
    o_file = tmpfile();


    EXPECT_EQ(0, echo_cmd.run(echo_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));
}


TEST(TestPwd, pwd) {
    FILE* i_file = nullptr, *o_file = nullptr, *e_file = tmpfile();
    token pwd_job = {"pwd", {"abc", "someone"}};
    auto env = std::make_shared<Environment>();
    env->emplace("PWD", std::filesystem::current_path().string());
    Pwd pwd_cmd;
    o_file = tmpfile();

    std::string expected = std::filesystem::current_path().string() + '\n';
    EXPECT_EQ(0, pwd_cmd.run(pwd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    fclose(o_file);
    o_file = tmpfile();
    pwd_job = {"pwd", {}};
    EXPECT_EQ(0, pwd_cmd.run(pwd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));
}


TEST(TestCat, cat) {
    FILE* i_file = nullptr, *o_file = nullptr, *e_file = tmpfile();
    token cat_job = {"cat", {"./data/test_wc_1.txt"}};
    auto env = std::make_shared<Environment>();
    env->emplace("PWD", std::filesystem::current_path().string());
    Cat cat_cmd;
    o_file = tmpfile();

    std::string expected = "some text\n"
                           "cat cat cat cat\n"
                           "dog dog dog\n"
                           "\n"
                           "or cat?\n";

    EXPECT_EQ(0, cat_cmd.run(cat_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    fclose(o_file);
    o_file = tmpfile();
    cat_job = {"cat", {"./data/test_wc_1.txt", "./data/test_wc_2.txt"}};
    expected = "some text\n"
               "cat cat cat cat\n"
               "dog dog dog\n"
               "\n"
               "or cat?\n"
               "Somebody once told me the world is gonna roll me\n"
               "I ain't the sharpest tool in the shed\n";

    EXPECT_EQ(0, cat_cmd.run(cat_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    fclose(o_file);
    o_file = tmpfile();
    cat_job = {"cat", {"./data/test_wc_1.txt", "error"}};
    expected = "some text\n"
               "cat cat cat cat\n"
               "dog dog dog\n"
               "\n"
               "or cat?\n"
               "cat: error: No such file or directory\n";

    EXPECT_EQ(1, cat_cmd.run(cat_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

//    fclose(o_file);
//    o_file = tmpfile();
//    cat_job = {"cat", {}};
//    expected = "No files were transferred";
//    try {
//        cat_cmd.run(cat_job,  env, i_file, o_file, e_file);
//    } catch(const std::invalid_argument& err) {
//        EXPECT_EQ(expected, err.what());
//    } catch(...) {
//        FAIL() << "expected std::invalid_argument";
//    }
}


TEST(TestWc, wc) {
    FILE* i_file = nullptr, *o_file = nullptr, *e_file = tmpfile();
    token wc_job = {"wc", {"./data/test_wc_1.txt"}};
    auto env = std::make_shared<Environment>();
    env->emplace("PWD", std::filesystem::current_path().string());
    Wc wc_cmd;
    o_file = tmpfile();

    std::string expected = " 5 11 47 ./data/test_wc_1.txt\n";

    EXPECT_EQ(0, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

    wc_job = {"wc", {"./data/test_wc_1.txt", "./data/test_wc_2.txt"}};
    expected = "  5  11  47 ./data/test_wc_1.txt\n"
               "  2  18  87 ./data/test_wc_2.txt\n"
               "  7  29 134 total\n";

    fclose(o_file);
    o_file = tmpfile();
    EXPECT_EQ(0, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

    wc_job = {"wc", {"./data/test_wc_1.txt", "error"}};

    expected = " 5 11 47 ./data/test_wc_1.txt\n"
               "error: No such file or directory\n"
               " 5 11 47 total\n";

    fclose(o_file);
    o_file = tmpfile();
    EXPECT_EQ(1, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

//    fclose(o_file);
//    o_file = tmpfile();
//    wc_job = {"wc", {}};
//    expected = "No files were transferred";
//    try {
//        wc_cmd.run(wc_job, env, i_file, o_file, e_file);
//    } catch(const std::invalid_argument& err) {
//        EXPECT_EQ(expected, err.what());
//    } catch(...) {
//        FAIL() << "expected std::invalid_argument";
//    }
}