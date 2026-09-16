#pragma once

#include <string>


namespace fela::encryption
{

    [[nodiscard]] std::string generate_token();
    [[nodiscard]] std::string encrypt_token(const std::string& _raw_token);

    [[nodiscard]] std::string encrypt_password(const std::string& _password);
    [[nodiscard]] bool verify_password(const std::string& _password, const std::string& _stored_hash);
} // fela
