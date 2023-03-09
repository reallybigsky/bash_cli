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

    Application app(argc, argv);
    app.run();

    return 0;
}