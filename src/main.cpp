#include "application.hpp"
#include "common.hpp"

#include <iostream>

#include <boost/process/io.hpp>
#include <boost/process/system.hpp>
#include <boost/asio.hpp>
#include <boost/iostreams/copy.hpp>

namespace bp = boost::process;
namespace io = boost::iostreams;

int main(int argc, char* argv[]) {

//    Application app(argc, argv);
//    app.run();

//    std::fstream ifs("input.txt");
//    std::fstream ofs("output.txt");

//    std::stringstream ss1;
//    std::stringstream ss2;
//
//    fun(ifs, ss1);
//    fun(ss1, ss2);
//    fun(ss2, ofs);

//    size_t cnt = 0;
//    char* line = nullptr;
//    size_t read = getline(&line, &cnt, stdin);
//    std::cout << line << std::endl << cnt << '\t' << read << std::endl;
//
//    std::cout << ftell(stdin) << std::endl;
//    std::cout << ftell(stdout) << std::endl;
//
//    FILE* f = fopen("output.txt", "r");
//    std::cout << ftell(f) << std::endl;
//    read = getline(&line, &cnt, f);
//    fread(line, 1, 5, f);
//    std::string_view sv(line, 5);
//    std::cout << sv << std::endl << cnt << '\t' << read << std::endl;
//    std::cout << ftell(f) << std::endl;
//    free(line);
//
//    std::ifstream ifs("output.txt");
//    ifs.seekg(read + 5);
//    std::string str;
//    ifs >> str;
//    std::cout << str;
//
//    fclose(f);


//    bp::pstream p1;
//    p1 << "hello" << std::endl;
//    std::string tmp;
//    p1 >> tmp;
//
//    std::cout << tmp;

//    std::cout << tmp1 << std::endl;
    FILE* fi = fopen("input.txt", "r");
    FILE* fo = fopen("output.txt", "w");
    std::cout << ftell(fi) << "\t" << ftell(fo) << std::endl;
    bp::system("wc", bp::std_out > fo, bp::std_in < fi);
    std::cout << ftell(fi) << "\t" << ftell(fo) << std::endl;
//    std::cout << "Hello" << std::endl;
//    std::cout << op.rdbuf() << std::endl;
    return 0;
}