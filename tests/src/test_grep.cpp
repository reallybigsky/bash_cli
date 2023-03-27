#include <gtest/gtest.h>
#include "file_utils.hpp"
#include "grep.hpp"


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

std::string read_file_to_string(FILE* out){
    rewind(out);
    std::stringstream result;
    while (auto line = FileUtils::readLine(out))
        result << line.value();

    rewind(out);

    return result.str();
}

void create_testfile(const std::string& filename, const std::vector<std::string>& file_content) {
    std::fstream f1(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
    if (f1.is_open()) {
        for (const auto& s : file_content) {
            f1 << s << std::endl;
        }
        f1.close();
    } else
        std::cout << filename << "NOT OPENED" << std::endl;
}

TEST(TestGrepBase, grep_base) {
    FILE* i_file = nullptr, *o_file = nullptr, *e_file = tmpfile();
    std::string filepath1 = "test1.txt", filepath2 = "test2.txt";
    create_testfile(filepath1, file_content1);
    create_testfile(filepath2, file_content2);


    token grep_job = {"grep", {"text", filepath1}};
    auto env = std::make_shared<Environment>();
    env->emplace("PWD", std::filesystem::current_path().string());
    Grep grep_cmd;
    o_file = tmpfile();
    std::string expected;

    expected = "some text\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));
    fclose(o_file);

    remove(filepath1.c_str());
    remove(filepath2.c_str());
}
