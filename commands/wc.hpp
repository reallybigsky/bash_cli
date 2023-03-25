#ifndef BASH_CLI_WC_HPP
#define BASH_CLI_WC_HPP

#include "cmd.hpp"
#include "file_utils.hpp"


namespace commands {

/**
 * Implementation of wc command
 */
class Wc : public Cmd {

private:
    struct answer_format {
        size_t count_of_lines_;
        size_t count_of_words_;
        size_t size_;
        std::string filename_;
        std::string error_;

        answer_format(std::string err, std::string filename)
            : count_of_lines_(0)
            , count_of_words_(0)
            , size_(0)
            , filename_(std::move(filename))
            , error_(std::move(err))
        {}

        answer_format(size_t count_of_lines, size_t count_of_words,
                      size_t size, std::string filename)
            : count_of_lines_(count_of_lines)
            , count_of_words_(count_of_words)
            , size_(size)
            , filename_(std::move(filename))
        {}

        bool is_error() const noexcept { return !error_.empty(); }

        size_t get_max_size() const noexcept {
            size_t max_value = std::max({count_of_lines_, count_of_words_, size_});
            return std::to_string(max_value).size();
        }

        std::string get_error() const noexcept {
            return filename_ + error_;
        }

        std::string get_formatted_values(size_t field_size) const {
            std::stringstream result;
            if (is_error()) {
                result << get_error();
            } else {
                result << std::right << std::setw(field_size) << count_of_lines_ << " "
                       << std::right << std::setw(field_size) << count_of_words_ << " "
                       << std::right << std::setw(field_size) << size_;
                result << " " << filename_;
            }

            return result.str();
        }
    };

public:
    /**
     * Calculates the number of words, lines, and bytes in the input and outputs to the output stream.
     * How it is written in interpreter syntax:  <wc file*>
     *
     * If token.args is empty, then the reading is from the input stream
     *
     * @param params: token with command name in tok.name and command arguments in tok.args
     * @param env: current environment variables of the interpreter
     * @param input: input FILE stream
     * @param output: output FILE stream
     * @param err: error FILE stream (unused)
     * @return 0 if there were no errors, 1 otherwise
     *
     * @throws std::invalid_arguments: Thrown if all files in tok.args not exists or cannot be open
     */
    virtual int run(const token& params, std::shared_ptr<Environment> env, FILE* input, FILE* output, FILE* err) override {
        uint64_t total_cnt_lines = 0, total_cnt_words = 0, total_size = 0;

        if (params.args.empty()) {
            while (auto line = FileUtils::readLine(input)) {
                ++total_cnt_lines;
                total_cnt_words += count_of_words_in_string(line.value());
                total_size += line.value().size();
            }

            std::stringstream result;
            result << total_cnt_lines << "\t" << total_cnt_words << "\t" << total_size << std::endl;
            FileUtils::writeToFile(result.str(), output);
            return 0;
        }

        std::vector<answer_format> result;
        std::stringstream errors;
        int32_t error_count = 0;

        for (auto &filename: params.args) {
            std::filesystem::path current_path(env->at("PWD").to_string());
            current_path /= filename;

            // проверка на то, существует ли файл в текущей директории
            if (!FileUtils::is_file_exist(current_path)) {
                // существует ли файл, если заданный путь полный
                if (!FileUtils::is_file_exist(filename)) {
                    ++error_count;
                    result.emplace_back(": No such file or directory", filename);
                    errors << result.back().get_error() << std::endl;
                    continue;
                }
                current_path = filename;
            }
            // проверка на возможность чтения из файла
            if (!FileUtils::is_readable(current_path)) {
                ++error_count;
                result.emplace_back(": Permission denied", filename);
                errors << result.back().get_error() << std::endl;
                continue;
            }

            // подсчет количества строк, слов, размера и обновление суммарной статистики
            size_t cnt_lines = get_count_of_lines(current_path);
            size_t cnt_words = get_count_of_words(current_path);
            size_t size = get_size(current_path);
            total_cnt_lines += cnt_lines;
            total_cnt_words += cnt_words;
            total_size += size;

            result.emplace_back(cnt_lines, cnt_words, size, filename);
        }

        // если была подсчитана хоть одна статистика, то добавляется поле total
        if (params.args.size() > 1 && error_count < params.args.size()) {
            result.emplace_back(total_cnt_lines, total_cnt_words, total_size, "total");
        }

        // создание форматированной результирующей строки
        std::string output_str = create_output_format(result);

        // если по всем аргументам произошли ошибки, то выбрасывается исключение
        if (error_count == params.args.size())
            throw std::invalid_argument(output_str);

        FileUtils::writeToFile(output_str, output);

        if(!errors.str().empty()) {
            FileUtils::writeToFile(errors.str(), err);
        }

        if (error_count > 0) {
            return 1;
        }

        return 0;
    }

private:

    static size_t count_of_words_in_string(const std::string& str) {
        std::stringstream stream(str);
        return std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
    }

    static std::string create_output_format(const std::vector<answer_format> &result) {
        size_t max_filead_size = 0;
        for (auto &file_stat: result) {
            max_filead_size = std::max(max_filead_size, file_stat.get_max_size());
        }

        std::stringstream str_result;
        for (auto &file_stat: result) {
            str_result << file_stat.get_formatted_values(max_filead_size) << std::endl;
        }

        return str_result.str();
    }

    static uint64_t get_count_of_lines(const std::filesystem::path &filepath) {
        std::ifstream file(filepath);
        return std::count(std::istreambuf_iterator<char>(file),
                          std::istreambuf_iterator<char>(), '\n');
    }

    static uint64_t get_size(const std::filesystem::path &filepath) {
        return std::filesystem::file_size(filepath);
    }

    static uint64_t get_count_of_words(const std::filesystem::path &filepath) {
        std::ifstream file(filepath);
        return std::distance(std::istream_iterator<std::string>(file),
                             std::istream_iterator<std::string>());
    }
};

} // namespace commands

#endif //BASH_CLI_WC_HPP
