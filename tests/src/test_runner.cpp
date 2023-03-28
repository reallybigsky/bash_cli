#include "application.hpp"
#include <gtest/gtest.h>


const std::vector<std::string> input_substitution_content = {
       "a=substitution\n",
       "echo $a\n",
       "\n",
       "echo '$a'\n",
       "echo '\\$a'\n",
       "\n",
       "echo \"$a\"\n",
       "echo \"\\$a\"\n",
       "\n",
       "echo '\"$a\"'\n",
       "echo '\"\\$a\"'\n",
       "\n",
       "echo \"'$a'\"\n",
       "echo \"'\\$a'\"\n",
       "\n",
       "say=echo\n",
       "$say test alias\n",
       "$say $a\n",
       "\n",
       "a=I\n",
       "a2=$a$a\n",
       "a4=$a2$a2\n",
       "echo $a4$a4\n",
       "\n",
       "a=ec\n",
       "b=ho\n",
       "$a$b composed command\n",
       "\n",
       "echo $?\n",
       "\n",
       "exit"
};

const std::vector<std::string> expected_content = {
        "substitution\n",
        "$a\n",
        "\\$a\n",
        "substitution\n",
        "$a\n",
        "\"$a\"\n",
        "\"\\$a\"\n",
        "'substitution'\n",
        "'$a'\n",
        "test alias\n",
        "substitution\n",
        "IIIIIIII\n",
        "composed command\n",
        "0"
};

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
    Application app(4, input_streams);
    app.run();

    std::vector<std::string> result;
    std::fstream out(filepath2);
    std::string line;
    while(std::getline(out, line)) {
        if (!line.empty()) {
            result.push_back(line);
        }
    }

    EXPECT_EQ(result.size(), expected_content.size());
    for(size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i], expected_content[i]);
    }

    remove(filepath1.c_str());
    remove(filepath2.c_str());
    remove(filepath3.c_str());
}

//int main(int argc, char* argv[]) {
//    Application app(argc, argv);
//    app.run();
//    return 0;
//}