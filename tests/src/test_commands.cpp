#include <gtest/gtest.h>
#include "file_utils.hpp"
#include "echo.hpp"
#include "pwd.hpp"
#include "cat.hpp"
#include "wc.hpp"
#include <stdio.h>
#include <boost/filesystem.hpp>


using namespace commands;

std::string read_file_to_string(FILE* out){
    rewind(out);
    std::stringstream result;
    while (auto line = FileUtils::readLine(out))
        result << line.value();

    rewind(out);

    return result.str();
}

const std::string file_content1 = "some text\n"
                                  "cat cat cat cat\n"
                                  "dog dog dog\n"
                                  "\n"
                                  "or cat?\n";
const std::string file_content2 =  "Somebody once told me the world is gonna roll me\n"
                                   "I ain't the sharpest tool in the shed\n";


void create_testfile(const std::string& filename, const std::string& file_content) {
    std::fstream f1(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
    if (f1.is_open()) {
        f1 << file_content;
        f1.close();
    } else
        std::cout << filename << "NOT OPENED" << std::endl;
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
    fclose(o_file);
    fclose(e_file);
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
    fclose(o_file);
    fclose(e_file);
}


TEST(TestCat, cat) {
    FILE* i_file = nullptr, *o_file = nullptr, *e_file = tmpfile();
    std::string filepath1 = "test1.txt", filepath2 = "test2.txt";
    create_testfile(filepath1, file_content1);
    create_testfile(filepath2, file_content2);


    token cat_job = {"cat", {filepath1}};
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
    cat_job = {"cat", {filepath1, filepath2}};
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
    cat_job = {"cat", {filepath1, "error"}};
    expected = "some text\n"
               "cat cat cat cat\n"
               "dog dog dog\n"
               "\n"
               "or cat?\n"
               "cat: error: No such file or directory\n";

    EXPECT_EQ(1, cat_cmd.run(cat_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));
    fclose(o_file);
    fclose(e_file);

    remove(filepath1.c_str());
    remove(filepath2.c_str());
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
    std::string filepath1 = "test1.txt", filepath2 = "test2.txt";
    create_testfile(filepath1, file_content1);
    create_testfile(filepath2, file_content2);

    token wc_job = {"wc", {filepath1}};
    auto env = std::make_shared<Environment>();
    env->emplace("PWD", std::filesystem::current_path().string());
    Wc wc_cmd;
    o_file = tmpfile();

    std::string expected = " 5 11 47 test1.txt\n";

    EXPECT_EQ(0, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

    wc_job = {"wc", {filepath1, filepath2}};
    expected = "  5  11  47 test1.txt\n"
               "  2  18  87 test2.txt\n"
               "  7  29 134 total\n";

    fclose(o_file);
    o_file = tmpfile();
    EXPECT_EQ(0, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

    wc_job = {"wc", {filepath1, "error"}};

    expected = " 5 11 47 test1.txt\n"
               "error: No such file or directory\n"
               " 5 11 47 total\n";

    fclose(o_file);
    o_file = tmpfile();
    EXPECT_EQ(1, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));
    fclose(o_file);
    fclose(e_file);

    remove(filepath1.c_str());
    remove(filepath2.c_str());

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