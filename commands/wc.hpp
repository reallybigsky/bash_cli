#pragma once

#include "cmd.hpp"
#include "file_utils.hpp"


namespace Commands {

/**
 * Implementation of wc command
 */
class Wc : public Cmd {
private:
    struct AnswerFormat {
        size_t count_of_lines_;
        size_t count_of_words_;
        size_t size_;
        std::string filename_;
        std::string error_;

        AnswerFormat(std::string filename, std::string err)
            : count_of_lines_(0)
            , count_of_words_(0)
            , size_(0)
            , filename_(std::move(filename))
            , error_(std::move(err))
        {}

        AnswerFormat(size_t count_of_lines, size_t count_of_words,
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
     * If params.args is empty, then the reading is from the input stream
     *
     * @param params: CmdToken with command name in params.name and command arguments in params.args
     * @param env: current environment of the interpreter
     * @param input: input FileStream
     * @param output: output FileStream
     * @param err: error FileStream
     * @return 0 if there were no errors, 1 otherwise
     *
     */
    virtual int run(const CmdToken& params, std::shared_ptr<Environment> env, FileStream& input, FileStream& output, FileStream& err) const override {
        uint64_t total_cnt_lines = 0;
        uint64_t total_cnt_words = 0;
        uint64_t total_size = 0;

        if (params.args.empty()) {
            while (auto line = input.read_line()) {
                ++total_cnt_lines;
                total_cnt_words += count_of_words_in_string(line.value());
                total_size += line.value().size();
            }

            output << std::to_string(total_cnt_lines) << "\t" << std::to_string(total_cnt_words) << "\t" << std::to_string(total_size) << "\n";
            return 0;
        }

        std::vector<AnswerFormat> result;
        std::stringstream result_stream;
        size_t error_counter = 0;

        for (auto& filename: params.args) {

            auto result_validation = file_validation_check(result_stream, params.name, env->current_path, filename, error_counter);
            if (!result_validation.error_message.empty()) {
                result.emplace_back(filename, result_validation.error_message);
                continue;
            }

            // подсчет количества строк, слов, размера и обновление суммарной статистики
            size_t cnt_lines = get_count_of_lines(result_validation.full_filepath);
            size_t cnt_words = get_count_of_words(result_validation.full_filepath);
            size_t size = get_size(result_validation.full_filepath);
            total_cnt_lines += cnt_lines;
            total_cnt_words += cnt_words;
            total_size += size;

            result.emplace_back(cnt_lines, cnt_words, size, filename);
        }

        // если была подсчитано больше одной статистики, то добавляется поле total
        if (params.args.size() > 1 && error_counter < params.args.size()) {
            result.emplace_back(total_cnt_lines, total_cnt_words, total_size, "total");
        }

        // если по всем аргументам произошли ошибки, то выбрасывается исключение
        if (error_counter == params.args.size()) {
            err << result_stream.str();
            return 2;
        }
        // создание форматированной результирующей строки
        output << create_output_format(result);

        if (error_counter > 0) {
            return 1;
        }

        return 0;
    }

private:

    static size_t count_of_words_in_string(const std::string& str) {
        std::stringstream stream(str);
        return std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
    }

    static std::string create_output_format(const std::vector<AnswerFormat>& result) {
        size_t max_fileaf_size = 0;
        for (auto& file_stat: result) {
            max_fileaf_size = std::max(max_fileaf_size, file_stat.get_max_size());
        }

        std::stringstream str_result;
        for (auto& file_stat: result) {
            str_result << file_stat.get_formatted_values(max_fileaf_size) << std::endl;
        }

        return str_result.str();
    }

    static uint64_t get_count_of_lines(const std::filesystem::path& filepath) {
        std::ifstream file(filepath);
        return std::count(std::istreambuf_iterator<char>(file),
                          std::istreambuf_iterator<char>(), '\n');
    }

    static uint64_t get_size(const std::filesystem::path& filepath) {
        return std::filesystem::file_size(filepath);
    }

    static uint64_t get_count_of_words(const std::filesystem::path& filepath) {
        std::ifstream file(filepath);
        return std::distance(std::istream_iterator<std::string>(file),
                             std::istream_iterator<std::string>());
    }
};

} // namespace commands
