#include "test_utils.hpp"
#include "echo.hpp"
#include "pwd.hpp"
#include "cat.hpp"
#include "wc.hpp"
using namespace Commands;

const std::vector<std::string> file_content1 = {
    "some text",
    "cat cat cat cat",
    "dog dog dog",
    "",
    "or cat?",
};

const std::vector<std::string> file_content2 = {
        "Somebody once told me the world is gonna roll me",
        "I ain't the sharpest tool in the shed",
};

TEST(TestEcho, echo) {
    FileStream i_file;
    FileStream o_file;
    FileStream e_file;
    CmdToken echo_job = {"echo", {"abc", "testtt", "someone"}};
    auto env = std::make_shared<Environment>();
    Echo echo_cmd;

    std::string expected = "abc testtt someone\n";
    EXPECT_EQ(0, echo_cmd.run(echo_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    echo_job = {"echo", {}};
    expected = "\n";
    o_file.reset();

    EXPECT_EQ(0, echo_cmd.run(echo_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));
}

TEST(TestPwd, pwd) {
    FileStream i_file;
    FileStream o_file;
    FileStream e_file;
    CmdToken pwd_job = {"pwd", {"abc", "someone"}};
    auto env = std::make_shared<Environment>();
    Pwd pwd_cmd;

    EXPECT_EQ(0, pwd_cmd.run(pwd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(std::filesystem::path(std::filesystem::current_path().string() + '\n', std::filesystem::path::format::generic_format),
              std::filesystem::path(read_file_to_string(o_file), std::filesystem::path::format::generic_format));

    o_file.reset();

    pwd_job = {"pwd", {}};
    EXPECT_EQ(0, pwd_cmd.run(pwd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(std::filesystem::path(std::filesystem::current_path().string() + '\n', std::filesystem::path::format::generic_format),
              std::filesystem::path(read_file_to_string(o_file), std::filesystem::path::format::generic_format));
}

TEST(TestCat, cat) {
    FileStream i_file;
    FileStream o_file;
    std::string filepath1 = "test1.txt", filepath2 = "test2.txt";
    create_testfile(filepath1, file_content1);
    create_testfile(filepath2, file_content2);

    CmdToken cat_job = {"cat", {filepath1}};
    auto env = std::make_shared<Environment>();
    Cat cat_cmd;

    std::string expected = "some text\n"
                           "cat cat cat cat\n"
                           "dog dog dog\n"
                           "\n"
                           "or cat?\n";

    EXPECT_EQ(0, cat_cmd.run(cat_job,  env, i_file, o_file, o_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    cat_job = {"cat", {filepath1, filepath2}};
    expected = "some text\n"
               "cat cat cat cat\n"
               "dog dog dog\n"
               "\n"
               "or cat?\n"
               "Somebody once told me the world is gonna roll me\n"
               "I ain't the sharpest tool in the shed\n";

    EXPECT_EQ(0, cat_cmd.run(cat_job,  env, i_file, o_file, o_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    cat_job = {"cat", {filepath1, "error"}};
    expected = "some text\n"
               "cat cat cat cat\n"
               "dog dog dog\n"
               "\n"
               "or cat?\n"
               "cat: error: No such file or directory\n";

    EXPECT_EQ(0, cat_cmd.run(cat_job,  env, i_file, o_file, o_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    cat_job = {"cat", {"error1", "error2"}};
    expected = "cat: error1: No such file or directory\n"
               "cat: error2: No such file or directory\n";

    EXPECT_EQ(1, cat_cmd.run(cat_job,  env, i_file, o_file, o_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    remove(filepath1.c_str());
    remove(filepath2.c_str());
}

TEST(TestWc, wc) {
    FileStream i_file;
    FileStream o_file;
    FileStream e_file;
    std::string filepath1 = "test1.txt", filepath2 = "test2.txt";
    create_testfile(filepath1, file_content1);
    create_testfile(filepath2, file_content2);

    CmdToken wc_job = {"wc", {filepath1}};
    auto env = std::make_shared<Environment>();
    Wc wc_cmd;

    std::filesystem::path t1 = std::filesystem::current_path() / filepath1;
    std::filesystem::path t2 = std::filesystem::current_path() / filepath2;

    std::string expected = " 5 11 " + std::to_string(std::filesystem::file_size(t1)) + " test1.txt\n";

    EXPECT_EQ(0, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

    wc_job = {"wc", {filepath1, filepath2}};
    expected = "  5  11  " + std::to_string(std::filesystem::file_size(t1)) + " test1.txt\n"
               "  2  18  " + std::to_string(std::filesystem::file_size(t2)) + " test2.txt\n"
               "  7  29 "  + std::to_string(std::filesystem::file_size(t1) + std::filesystem::file_size(t2)) + " total\n";

    o_file.reset();
    EXPECT_EQ(0, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

    wc_job = {"wc", {filepath1, "error"}};

    expected = " 5 11 " + std::to_string(std::filesystem::file_size(t1)) + " test1.txt\n"
               "error: No such file or directory\n"
               " 5 11 " + std::to_string(std::filesystem::file_size(t1)) + " total\n";

    o_file.reset();
    EXPECT_EQ(0, wc_cmd.run(wc_job, env, i_file, o_file, o_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

    o_file.reset();
    wc_job = {"wc", {"error1", "error2"}};
    expected = "wc: error1: No such file or directory\n"
               "wc: error2: No such file or directory\n";

    EXPECT_EQ(1, wc_cmd.run(wc_job,  env, i_file, o_file, o_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    remove(filepath1.c_str());
    remove(filepath2.c_str());
}