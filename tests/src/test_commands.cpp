#include "test_utils.hpp"
#include "echo.hpp"
#include "pwd.hpp"
#include "cat.hpp"
#include "wc.hpp"
#include "ls.hpp"
#include "cd.hpp"

using namespace commands;

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

    EXPECT_EQ(0, pwd_cmd.run(pwd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(std::filesystem::path(std::filesystem::current_path().string() + '\n', std::filesystem::path::format::generic_format),
              std::filesystem::path(read_file_to_string(o_file), std::filesystem::path::format::generic_format));

    fclose(o_file);
    o_file = tmpfile();
    pwd_job = {"pwd", {}};
    EXPECT_EQ(0, pwd_cmd.run(pwd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(std::filesystem::path(std::filesystem::current_path().string() + '\n', std::filesystem::path::format::generic_format),
              std::filesystem::path(read_file_to_string(o_file), std::filesystem::path::format::generic_format));
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
    o_file = tmpfile();
    cat_job = {"cat", {"error1", "error2"}};
    expected = "cat: error1: No such file or directory\n"
               "cat: error2: No such file or directory\n";

    EXPECT_EQ(1, cat_cmd.run(cat_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(e_file));

    fclose(o_file);
    fclose(e_file);

    remove(filepath1.c_str());
    remove(filepath2.c_str());
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

    std::filesystem::path t1 = std::filesystem::current_path() / filepath1;
    std::filesystem::path t2 = std::filesystem::current_path() / filepath2;

    std::string expected = " 5 11 " + std::to_string(std::filesystem::file_size(t1)) + " test1.txt\n";

    EXPECT_EQ(0, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

    wc_job = {"wc", {filepath1, filepath2}};
    expected = "  5  11  " + std::to_string(std::filesystem::file_size(t1)) + " test1.txt\n"
               "  2  18  " + std::to_string(std::filesystem::file_size(t2)) + " test2.txt\n"
               "  7  29 "  + std::to_string(std::filesystem::file_size(t1) + std::filesystem::file_size(t2)) + " total\n";

    fclose(o_file);
    o_file = tmpfile();
    EXPECT_EQ(0, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));

    wc_job = {"wc", {filepath1, "error"}};

    expected = " 5 11 " + std::to_string(std::filesystem::file_size(t1)) + " test1.txt\n"
               "error: No such file or directory\n"
               " 5 11 " + std::to_string(std::filesystem::file_size(t1)) + " total\n";

    fclose(o_file);
    o_file = tmpfile();
    EXPECT_EQ(1, wc_cmd.run(wc_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected,  read_file_to_string(o_file));
    fclose(o_file);

    o_file = tmpfile();
    wc_job = {"wc", {"error1", "error2"}};
    expected = "error1: No such file or directory\n"
               "error2: No such file or directory\n";

    EXPECT_EQ(2, wc_cmd.run(wc_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(e_file));

    fclose(o_file);
    fclose(e_file);

    remove(filepath1.c_str());
    remove(filepath2.c_str());
}

TEST(TestLs, ls) {
    FILE *i_file = nullptr, *o_file = nullptr, *e_file = tmpfile();
    std::string filepath1 = "test_dir_ls/test_ls.txt", filepath2 = "test_dir_ls/test_ls2.txt";
    std::filesystem::create_directory("test_dir_ls");
    std::filesystem::create_directory("test_dir_ls/dir");
    create_testfile(filepath1, file_content1);
    create_testfile(filepath2, file_content2);

    auto env = std::make_shared<Environment>();
    env->emplace("PWD", std::filesystem::current_path().string());

    token ls_job = {"ls", {"test_dir_ls"}};

    Ls ls;
    o_file = tmpfile();

    std::string expected;
    #if defined(_WIN64) || defined(_WIN32)
    expected = std::filesystem::path("dir/",
                                                 std::filesystem::path::format::generic_format).string() + '\n'
                           + "test_ls.txt\n"
                           "test_ls2.txt\n\n";
    #else
    expected = "test_ls2.txt\n"
               "test_ls.txt\n"
               + std::filesystem::path("dir/",
                                     std::filesystem::path::format::generic_format).string() + "\n\n";
    #endif

    EXPECT_EQ(0, ls.run(ls_job, env, i_file, o_file, e_file));
    EXPECT_EQ(std::filesystem::path(expected,  std::filesystem::path::format::generic_format),
              std::filesystem::path(read_file_to_string(o_file),  std::filesystem::path::format::generic_format));

    fclose(o_file);
    e_file = tmpfile();

    ls_job = {"ls", {"not_exist"}};

    expected = "ls: not_exist: No such directory\n";

    EXPECT_EQ(1, ls.run(ls_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(e_file));

    fclose(e_file);
}

TEST(TestCd, cd) {
    FILE *i_file = nullptr, *o_file = nullptr, *e_file = tmpfile();

    auto env = std::make_shared<Environment>();
    env->emplace("PWD", std::filesystem::current_path().string());
    Cd cd;

    std::string expected = "cd: too many arguments\n";
    token cd_job = {"cd", {"many", "arguments"}};

    EXPECT_EQ(1, cd.run(cd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(e_file));

    expected = "cd: error_directory: No such file or directory\n";
    cd_job = {"cd", {"error_directory"}};

    EXPECT_EQ(1, cd.run(cd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(e_file));

    fclose(e_file);
    e_file = tmpfile();

    std::string test_file = "test_file_for_cd";
    std::vector<std::string> content;
    create_testfile(test_file, content);

    expected = "cd: test_file_for_cd: Not a directory\n";
    cd_job = {"cd", {"test_file_for_cd"}};

    EXPECT_EQ(1, cd.run(cd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(e_file));

    fclose(e_file);
    o_file = tmpfile();

    std::filesystem::create_directory("some_dir");
    std::filesystem::path exp = std::filesystem::current_path();
    exp /= "some_dir\n";
    cd_job = {"cd", {"some_dir"}};

    EXPECT_EQ(0, cd.run(cd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(std::filesystem::path(exp, std::filesystem::path::format::generic_format),
              std::filesystem::path(read_file_to_string(o_file), std::filesystem::path::format::generic_format));

    fclose(o_file);

    o_file = tmpfile();

    expected = std::filesystem::current_path().root_path().string() + '\n';
    cd_job = {"cd", {}};

    EXPECT_EQ(0, cd.run(cd_job, env, i_file, o_file, e_file));
    EXPECT_EQ(std::filesystem::path(expected, std::filesystem::path::format::generic_format),
              std::filesystem::path(read_file_to_string(o_file), std::filesystem::path::format::generic_format));
    fclose(o_file);
}