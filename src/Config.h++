#pragma once
#include <filesystem>
#include <optional>
#include <unordered_map>

namespace fela
{
    struct Config
    {
        std::string db_uri_ = "postgresql://postgres:mleko1235@localhost:5432/auth_service";
        unsigned int port_ = 6767;
        bool use_ssl_ = true;
        std::string key_path_;
        std::string cert_path_;
    };

    std::string_view trim_whitespaces(std::string_view _str_view);
    std::optional<Config> load_config(const std::string& _file_path);
    std::optional<Config> parse_config(std::ifstream& _file_stream);
    bool update_config(Config& _config, const std::string_view& _key, const std::string_view& _val);
} // fela
