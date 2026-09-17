#include "Config.h++"

#include <fstream>
#include <iostream>

namespace fela
{
    constexpr char SEPARATOR = '=';
    constexpr char COMMENT_CHARACTER = '#';
    constexpr std::string_view WHITE_CHARACTERS = " \t\r\n";
    constexpr int EXPECTED_CONFIG_ENTRIES = 5;

    std::string_view trim_whitespaces(std::string_view _str_view)
    {
        const std::size_t start_pos = _str_view.find_first_not_of(WHITE_CHARACTERS);
        if (start_pos == std::string_view::npos) return {};
        const std::size_t end_pos = _str_view.find_last_not_of(WHITE_CHARACTERS);
        return _str_view.substr(start_pos, end_pos - start_pos + 1);
    }

    bool update_config(Config& _config, const std::string_view& _key, const std::string_view& _val)
    {
        if (_key == "db_uri")
        {
            _config.db_uri_ = _val;
            return true;
        }
        if (_key == "port")
        {
            try
            {
                _config.port_ = std::stoi(std::string(_val));
                return true;
            }
            catch (...)
            {
                std::cerr << "Config error: 'port' must be valid integer, got '" << _val << "'\n";
                return false;
            }
        }
        if (_key == "ssl")
        {
            if (_val == "true" || _val == "1")
            {
                _config.use_ssl_ = true;
                return true;
            }
            if (_val == "false" || _val == "0")
            {
                _config.use_ssl_ = false;
                return true;
            }
            std::cerr << "Config error: 'ssl' must be true/false or 1/0, got '" << _val << "'\n";
            return false;
        }
        if (_key == "key_path")
        {
            _config.key_path_ = _val;
            return true;
        }
        if (_key == "cert_path")
        {
            _config.cert_path_ = _val;
            return true;
        }

        std::cerr << "Config error: Unknown key '" << _key << "'\n";
        return false;
    }

    std::optional<Config> parse_config(std::ifstream& _file_stream)
    {
        Config config;
        int updates = 0;
        int line_number = 0;

        for (std::string line; std::getline(_file_stream, line);)
        {
            ++line_number;
            std::string_view trimmed_line = trim_whitespaces(line);

            if (trimmed_line.empty() || trimmed_line.starts_with(COMMENT_CHARACTER))
            {
                continue;
            }

            const size_t separator_pos = trimmed_line.find(SEPARATOR);
            if (separator_pos == std::string_view::npos)
            {
                std::cerr << "Config error (line " << line_number << "): Missing '=' in: " << trimmed_line << "\n";
                continue;
            }

            std::string_view key = trim_whitespaces(trimmed_line.substr(0, separator_pos));
            std::string_view value = trim_whitespaces(trimmed_line.substr(separator_pos + 1));

            if (update_config(config, key, value))
            {
                updates++;
            }
        }

        if (updates != EXPECTED_CONFIG_ENTRIES)
        {
            std::cerr << "Config warning: Expected " << EXPECTED_CONFIG_ENTRIES
                      << " entries, but loaded " << updates << "\n";
        }

        return config;
    }

    std::optional<Config> load_config(const std::string& _file_path)
    {
        std::ifstream file(_file_path);
        if (!file.is_open())
        {
            file.open("./fela.conf");
        }

        if (!file.is_open())
        {
            std::cerr << "Config error: Could not open '" << _file_path << "' or './fela.conf'\n";
            return std::nullopt;
        }

        return parse_config(file);
    }
} // fela
