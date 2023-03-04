#ifndef BASH_CLI_IOSERVICE_HPP
#define BASH_CLI_IOSERVICE_HPP

#include <iostream>
#include <string>

//TODO: documentation

class IOservice {
public:
    IOservice(std::istream& is, std::ostream& os, std::ostream& es) : in(is), out(os), err(es) {}

    std::istream& getInput() const { return in; }
    std::ostream& getOutput() const { return out; }
    std::ostream& getErr() const { return out; }

    void greet() const {
        out << "> ";
    }

    std::string readLine() const {
        std::string res;
        getline(in, res);
        return res;
    }

private:
    std::istream& in;
    std::ostream& out;
    std::ostream& err;
};

#endif //BASH_CLI_IOSERVICE_HPP
