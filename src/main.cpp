#include "application.hpp"

int main(int argc, const char* argv[]) {
    Application app(static_cast<size_t>(argc), std::span{argv, static_cast<size_t>(argc)});
    app.run();
    return 0;
}