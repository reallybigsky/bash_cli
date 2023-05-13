#include "test_utils.hpp"
#include "grep.hpp"


using namespace Commands;

const std::vector<std::string> file_content1 = {
        "Azure space is aflame up above,",
        "I've forgotten my home destination...",
        "For the first time I'm singing of love,",
        "For the first time I give up contention.",
        "",
        "I was all like a desolate grove",
        "Loving women and heavily drinking.",
        "I don\"t drink any more and don't love",
        "Like I did, living fast and unthinking.",
        "",
        "All I want is to look at the vast",
        "Of your gold-brown eyes, and, — oh, bother! —",
        "How I wish that, disliking your past,",
        "You would not like to go to another!",
        "",
        "Gentle step, graceful waist that you have!",
        "Oh if only you wer' able to tumble",
        "How a bully can really love,",
        "And how he can be timid and humble!",
        "",
        "All those pubs I would never attend,",
        "And my poems would all be forgotten,",
        "If you let me take hold of your hand",
        "And your hair, the colour of autumn.",
        "",
        "I would follow you ever, my dove,",
        "Be it distant or close destination...",
        "For the first time I'm singing of love,",
        "For the first time I give up contention."
};

const std::vector<std::string> file_content2 = {
        "Somebody once told me the world is gonna roll me",
        "I ain't the sharpest tool in the shed",
};

TEST(TestGrepBase, grep_base) {
    FileStream i_file;
    FileStream o_file;
    FileStream e_file;
    std::string filepath1 = "test1.txt", filepath2 = "test2.txt";
    create_testfile(filepath1, file_content1);
    create_testfile(filepath2, file_content2);


    auto env = std::make_shared<Environment>();
    Grep grep_cmd;
    std::string expected;

    token grep_job = {"grep", {"Azure", filepath1}};

    expected = "Azure space is aflame up above,\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    grep_job = {"grep", {"destination", filepath1}};
    expected =  "I've forgotten my home destination...\n"
                "Be it distant or close destination...\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    remove(filepath1.c_str());
    remove(filepath2.c_str());
}

TEST(TestGrepErrors, grep_errors) {
    FileStream i_file;
    FileStream o_file;
    FileStream e_file;

    auto env = std::make_shared<Environment>();
    Grep grep_cmd;
    std::string expected;
    token grep_job;

    grep_job = {"grep", {}};

    expected = "Usage: grep [OPTION]... PATTERNS [FILE]...\n"
               "Try 'grep --help' for more information.";
    EXPECT_EQ(1, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(e_file));

    o_file.reset();
    e_file.reset();

    grep_job = {"grep", {"pattern", "gg"}};
    expected = "grep: gg: No such file or directory\n";
    EXPECT_EQ(1, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(e_file));
}


TEST(TestGrepOptions, grep_options) {
    FileStream i_file;
    FileStream o_file;
    FileStream e_file;
    std::string filepath1 = "test1.txt", filepath2 = "test2.txt";
    create_testfile(filepath1, file_content1);
    create_testfile(filepath2, file_content2);

    auto env = std::make_shared<Environment>();
    Grep grep_cmd;
    std::string expected;
    token grep_job;

    grep_job = {"grep", {"-i", "azure", filepath1}};

    expected = "Azure space is aflame up above,\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    e_file.reset();
    grep_job = {"grep", {"-w", "az", filepath1}};

    expected = "";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    e_file.reset();

    grep_job = {"grep", {"-iw", "azure", filepath1}};
    expected = "Azure space is aflame up above,\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    e_file.reset();

    grep_job = {"grep", {"-A", "2", "Azu", filepath1}};
    expected =  "Azure space is aflame up above,\n"
                "I've forgotten my home destination...\n"
                "For the first time I'm singing of love,\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    e_file.reset();

    grep_job = {"grep", {"-wiA", "2", "azure", filepath1}};
    expected =  "Azure space is aflame up above,\n"
                "I've forgotten my home destination...\n"
                "For the first time I'm singing of love,\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    e_file.reset();

    grep_job = {"grep", {"azure", "-i", "-w", "-A", "2", filepath1}};
    expected =  "Azure space is aflame up above,\n"
                "I've forgotten my home destination...\n"
                "For the first time I'm singing of love,\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));


    o_file.reset();
    e_file.reset();

    grep_job = {"grep", {"-iwA", "2", "for", filepath1}};
    expected =   "For the first time I'm singing of love,\n"
                 "For the first time I give up contention.\n"
                 "\n"
                 "--\n"
                 "For the first time I'm singing of love,\n"
                 "For the first time I give up contention.\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    remove(filepath1.c_str());
    remove(filepath2.c_str());
}

TEST(TestPatternRegex, pattern_regex) {
    FileStream i_file;
    FileStream o_file;
    FileStream e_file;
    std::string filepath1 = "test1.txt", filepath2 = "test2.txt";
    create_testfile(filepath1, file_content1);
    create_testfile(filepath2, file_content2);

    auto env = std::make_shared<Environment>();
    Grep grep_cmd;
    std::string expected;
    token grep_job;

    o_file.reset();
    e_file.reset();

    grep_job = {"grep", {"^Azu", filepath1}};
    expected = "Azure space is aflame up above,\n";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    o_file.reset();
    e_file.reset();

    grep_job = {"grep", {"Azu$", filepath1}};
    expected = "";
    EXPECT_EQ(0, grep_cmd.run(grep_job,  env, i_file, o_file, e_file));
    EXPECT_EQ(expected, read_file_to_string(o_file));

    remove(filepath1.c_str());
    remove(filepath2.c_str());
}
