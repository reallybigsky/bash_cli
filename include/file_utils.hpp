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

    explicit FileStream(FILE* c_file) : file(c_file), fname(std::to_string(fileno(file))) {}

    FileStream(const FileStream&) = delete;
    FileStream& operator=(const FileStream&) = delete;

    FileStream(FileStream&& other) : fname(std::move(other.fname)) {
        std::swap(file, other.file);
    }

    FileStream& operator=(FileStream&& other) noexcept {
        if (this != &other) {
            close_buffer();
            file = other.file;
            other.file = nullptr;
            fname = std::move(other.fname);
        }
        return *this;
    }

    ~FileStream() {
        close_buffer();
    }

    /**
     * Get c underlying FILE*
     */
    FILE* to_FILE() const {
        return file;
    }

    /**
     * Flush streambuffer
     */
    void flush() {
        fflush(file);
    }

    /**
     * Close underlying file and filestream
     */
    void close_buffer() {
        if (file == stdin || file == stdout || file == stderr) {
            file = nullptr;
        } else if (file != nullptr) {
            fclose(file);
            file = nullptr;
        }
        fname.clear();
    }

    /**
     * Set stream position to the beginning of the underlying file
     */
    void seek_begin() {
        ::rewind(file);
    }

    /**
     * Close current buffer if it exists and reopen stream with new file
     * @param name: filename
     * @param mode: access mode
     */
    void reset(const std::string& name = "", std::string mode = "rw") {
        close_buffer();
        fname = name;
        if (fname.empty()) {
            file = tmpfile();
            fname = "tmpfile" + std::to_string(fileno(file));
        } else if (file = fopen(name.data(), mode.c_str()); !file) {
            throw std::invalid_argument("Cannot open file: \"" + fname + '\"');
        }
    }

    /**
     * Write string to current FILE
     *
     * @param str: string to write
     * @return 1 there was an error in writing, else 0
     */
    void write_to_file(const std::string& str) const {
        if (str.size() != fwrite(str.c_str(), sizeof(char), str.size(), file))
            throw std::runtime_error("Cannot write to " + fname);
    }

    /**
     * Read one line from current FILE
     *
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
