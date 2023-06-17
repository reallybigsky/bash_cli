#include "application.hpp"
#include "test_utils.hpp"


const std::vector<std::string> input_substitution_content = {
       "a=substitution",
       "echo $a",
       "echo '$a'",
       "echo '\\$a'",
       "echo \"$a\"",
       "echo \"\\$a\"",
       "echo '\"$a\"'",
       "echo '\"\\$a\"'",
       "echo \"'$a'\"",
       "echo \"'\\$a'\"",
       "say=echo",
       "$say test alias",
       "$say $a",
       "a=I",
       "a2=$a$a",
       "a4=$a2$a2",
       "echo $a4$a4",
       "a=ec",
       "b=ho",
       "$a$b composed command",
       "echo $?",
       "exit"
};

const std::vector<std::string> expected_substitution_content = {
        "substitution",
        "$a",
        "\\$a",
        "substitution",
        "$a",
        "\"$a\"",
        "\"\\$a\"",
        "'substitution'",
        "'$a'",
        "test alias",
        "substitution",
        "IIIIIIII",
        "composed command",
        "0"
};

TEST(TestSubstitution, test_substitution) {
    std::string filepath1 = "input_substitution.txt", filepath2 = "out_substitution.txt", filepath3 = "error.txt";
    create_testfile(filepath1, input_substitution_content);
    create_testfile(filepath2, {});
    create_testfile(filepath3, {});

    const char* name = "test";
    const char* input = "in=input_substitution.txt";
    const char* output = "out=out_substitution.txt";
    const char* err = "err=error.txt";

    const char* input_streams [4] = {name, input, output, err};
    Application app(4, input_streams, "");
    app.run();

    std::vector<std::string> result;
    std::ifstream out_file(filepath2);
    if (out_file.is_open()) {
        std::string line;
        while (std::getline(out_file, line)) {
            if (!line.empty()) {
                result.push_back(line);
            }
        }
    }

    EXPECT_EQ(result.size(), expected_substitution_content.size());
    for(size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i], expected_substitution_content[i]);
    }

    remove(filepath1.c_str());
    remove(filepath2.c_str());
    remove(filepath3.c_str());
}

const std::vector<std::string> input_pipeline_content {
    "echo \"pipe into cat\" | cat",
    "echo \"pipe into wc\" | wc",
    "echo \"pipe into cat into wc\" | cat | wc",
    "echo \"pipe int wc into cat\" | wc | cat",
    "echo echo \"pipe into echo\" | echo",
    "echo \"pipe into cat into cat into cat\" | cat | cat | cat",
    "cat test.txt | wc",
    "exit"
};

const std::vector<std::string> expected_pipeline_content {
    "pipe into cat",
    "1\t3\t13",
    "1\t5\t22",
    "1\t5\t21",
    "pipe into cat into cat into cat",
    "1\t4\t17"
};

const std::vector<std::string> test_file_content {
    "Text in the file"
};

TEST(TestPipeline, test_pipeline) {
    std::string filepath1 = "input_pipeline.txt", filepath2 = "out_pipeline.txt", filepath3 = "error.txt", test_filepath = "test.txt";
    create_testfile(filepath1, input_pipeline_content);
    create_testfile(filepath2, {});
    create_testfile(filepath3, {});
    create_testfile(test_filepath, test_file_content);

    const char* name = "test";
    const char* input = "in=input_pipeline.txt";
    const char* output = "out=out_pipeline.txt";
    const char* err = "err=error.txt";

    const char* input_streams [4] = {name, input, output, err};
    Application app(4, input_streams, "");
    app.run();

    std::vector<std::string> result;
    std::ifstream out_file(filepath2);
    if (out_file.is_open()) {
        std::string line;
        while (std::getline(out_file, line)) {
            if (!line.empty()) {
                result.push_back(line);
            }
        }
    }

    EXPECT_EQ(result.size(), expected_pipeline_content.size());
    for(size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i], expected_pipeline_content[i]);
    }

    remove(filepath1.c_str());
    remove(filepath2.c_str());
    remove(filepath3.c_str());
}
