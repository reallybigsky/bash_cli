#pragma once

#include "common.hpp"

#include <filesystem>
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>

/**
 * EndOfInputStream exception
 */
struct EndOfInputStream : public std::ios_base::failure {
    EndOfInputStream(const std::string& str) : std::ios_base::failure(str) {}
};


#if defined(_WIN64) || defined(_WIN32)
#include "getline_win.hpp"
#endif

namespace fs = std::filesystem;

class FileStream {
private:
    FILE* file = nullptr;
    std::string fname;

public:
    FileStream(const std::string& name = "", std::string mode = "rw") {
        reset(name, mode);
    }

//    FileStream() : file(tmpfile()), fname("tmpfile" + std::to_string(fileno(file))) {}
    explicit FileStream(FILE* c_file) : file(c_file), fname(std::to_string(fileno(file))) {}

    FileStream(const FileStream&) = delete;
    FileStream& operator=(const FileStream&) = delete;

    FileStream(FileStream&& other) : fname(std::move(other.fname)) {
        std::swap(file, other.file);
    }

    FileStream& operator=(FileStream&& other) noexcept {
        if (this != &other) {
            file = other.file;
            other.file = nullptr;
            fname = std::move(other.fname);
        }
        return *this;
    }

    ~FileStream() {
        if (file == stdin || file == stdout || file == stderr) {
            return;
        }
        clear();
    }

    FILE* to_FILE() const {
        return file;
    }

    void flush() {
        fflush(file);
    }

    void clearerr() {
        ::clearerr(file);
    }

    void clear() {
        if (file != nullptr) {
            fclose(file);
            file = nullptr;
        }
        fname.clear();
    }

    void rewind() {
        ::rewind(file);
    }

    void reset(const std::string& name = "", std::string mode = "rw") {
        clear();
        fname = name;
        if (fname.empty()) {
            file = tmpfile();
            fname = "tmpfile" + std::to_string(fileno(file));
        } else if (file = fopen(name.data(), mode.c_str()); !file) {
            throw std::invalid_argument("Cannot open file: \"" + fname + '\"');
        }
    }

    /**
     * Write string to FILE stream
     *
     * @param str: string to write
     * @param file: FILE stream where str will be written
     * @return 1 there was an error in writing, else 0
     */
    void write_to_file(const std::string& str) const {
        if (str.size() != fwrite(str.c_str(), sizeof(char), str.size(), file))
            throw std::runtime_error("Cannot write to " + fname);
    }

    /**
     * Read one line from file
     *
     * @param file: FILE stream to read from
     * @return string line from file if EOF or errors weren't encountered, std::nullopt otherwise
     */
    std::optional<std::string> read_line() const {
        char* line = nullptr;
        size_t cnt = 0;
        size_t read = getline(&line, &cnt, file);
        std::optional<std::string> res;
        if (read == std::numeric_limits<size_t>::max()) {
            res = std::nullopt;
        } else {
            res = std::string(line, read);
        }
        free(line);
        return res;
    }

    FileStream& operator<<(const std::string& str) {
        write_to_file(str);
        return *this;
    }

    FileStream& operator>>(std::string& str) {
        auto res = read_line();
        if (res) {
            str = std::move(res.value());
            return *this;
        }
        throw EndOfInputStream("");
    }

};
