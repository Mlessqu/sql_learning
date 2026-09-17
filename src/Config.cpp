#include "Config.h++"

#include <fstream>
#include <httplib.h>

namespace fela
{
    std::filesystem::path CONFIG_PATH = "fela.conf";

    constexpr char SEPARATOR = '=';
    constexpr char COMMENT_CHARACTER = '#';
    constexpr std::string WHITE_CHARACTERS = " '\t\r\n";


    std::string_view trim_whitespaces(std::string_view _str_view)
    {
        const std::size_t start_pos = _str_view.find_first_not_of(WHITE_CHARACTERS);
        if (start_pos == std::string_view::npos) return {};
        const std::size_t end_pos = _str_view.find_last_not_of(WHITE_CHARACTERS);
        return _str_view.substr(start_pos, end_pos - start_pos + 1);
    }


    std::optional<Config> parse_config(std::ifstream& _file_stream)
    {
        Config config;
        for (std::string line; std::getline(_file_stream, line);)
        {
            std::string_view trimmed_line = trim_whitespaces(line);
            const size_t separator_pos = line.find(SEPARATOR);


            if (trimmed_line.empty()) continue;
            if (trimmed_line.starts_with(COMMENT_CHARACTER)) continue;
            if (separator_pos == std::string_view::npos) continue;
            std::string_view key = trimmed_line.substr(0, separator_pos);
            std::string_view value = trimmed_line.substr(separator_pos + 1);
            update_config(key,value);
        }
    }


    bool update_config(Config& _config, const std::string_view& _key, const std::string_view& _val)
    {
        bool updated = false;
        if (_key == "db_uri")
        {
            _config.db_uri_ = _val;
            return true;
        }
        if (_key == "port")
        {
            std::string temp(_val);
            _config.port_ = std::stoi(temp);
            return true;
        }
        if (_key == "ssl")
        {
            if (_key == "true" || _key == "1")
            {
                _config.use_ssl_ = true;
                return true;
            }
            if (_key == "false" || _key == "0")
            {
                _config.use_ssl_ = false;
                return true;
            }
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
        return false;
    }


    std::optional<Config> load_config(const std::string& _file_path)
    {
        Config config;
        std::ifstream file;
        file.open(_file_path);
        if (file.is_open())
        {
        }
        else
        {
            file.open("./fela.conf");
        }
    }
} // fela
