#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <optional>

namespace Commands {

    struct result_file_validation {
        std::filesystem::path full_filepath = "";
        std::string error_message;
    };

    void  print_error_message(std::stringstream& error_stream, const std::string& command_name, const std::string& filename, const std::string& error_mesage, size_t& error_counter) {
        ++error_counter;
        error_stream << command_name << ": " << filename << error_mesage << std::endl;
    }

    void print_error_message(std::stringstream& error_stream, const std::string& command_name, const std::string& error_mesage, size_t& error_counter) {
        ++error_counter;
        error_stream << command_name << ": " << error_mesage << std::endl;
    }

    result_file_validation file_validation_check(std::stringstream& error_stream, const std::string& command_name, const std::filesystem::path& current_path, const std::string& filename, size_t& error_counter) {
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