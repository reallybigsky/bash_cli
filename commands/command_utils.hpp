#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <optional>
#include <iterator>
#include <concepts>

namespace Commands {

    /**
     * read data from file into vector
     * @param filename - name of file
     * @return vector of file lines
     */
    std::vector<std::string> read_whole_file( const std::filesystem::path& filename) {
        std::fstream file(filename);
        std::vector<std::string> file_content;
        std::string line;
        while(std::getline(file, line))
            file_content.push_back(line);

        return file_content;
    }

    template <typename Stream>
    concept is_stream = requires (Stream&& stream, const std::string& str) {
        stream << str;
    };

    /**
     * Create error message into stream and increase error counter
     * @param error_stream
     * @param command_name
     * @param filename
     * @param error_mesage
     * @param error_counter
     */

    template <typename Stream> requires is_stream<Stream>
    void print_error_message(Stream& error_stream, const std::string& command_name, const std::string& filename, const std::string& error_message, size_t& error_counter) {
        ++error_counter;
        error_stream << command_name << ": " << filename << error_message << "\n";
    }

    /**
     * Contains filepath and error message for file validation
     */
    struct result_file_validation {
        std::filesystem::path full_filepath = "";
        std::string error_message;
    };

    /**
     * Сhecks the file for existence and returns the path to it or an error message
     * @param error_stream output stream
     * @param command_name command name
     * @param current_path path to main directory
     * @param filename file name
     * @param error_counter error counter when executing a command
     * @return the full path to the file or the error message contained in the result_file_validation
     */

    template <typename Stream> requires is_stream<Stream>
    result_file_validation file_validation_check(Stream& error_stream, const std::string& command_name, const std::filesystem::path& current_path, const std::string& filename, size_t& error_counter) {
        result_file_validation result;
        if (filename.empty()) {
            result.error_message = ": No such file or directory";
            print_error_message(error_stream, command_name, "''", result.error_message, error_counter);
            return result;
        }

        result.full_filepath = current_path / filename;

        // проверка на то, существует ли файл в текущей директории
        if (!fs::exists(result.full_filepath)) {
            if (!fs::exists(filename)) {
                result.error_message = ": No such file or directory";
                print_error_message(error_stream, command_name, filename, result.error_message, error_counter);
                return result;
            }
            result.full_filepath = filename;
        }

        // проверка на то, можно ли открыть файл на чтение
        if (!std::ifstream( result.full_filepath).is_open()) {
            result.error_message =  ": Permission denied";
            print_error_message(error_stream, command_name, filename, result.error_message , error_counter);
            return result;
        }

        return result;
    }
}