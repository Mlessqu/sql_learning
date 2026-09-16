#pragma once
#include <optional>
#include <string>

//talks to: AuthorizationService,
//bundles helper functions that encrypts data
namespace fela::encryption
{


    std::string generate_token();
    //TODO: encrypt and verify token
    std::string encrypt_token();
    [[nodiscard]] bool verify_token();
    std::string encrypt_password(const std::string& _password);
    [[nodiscard]] bool verify_password(const std::string& _password, const std::string& _stored_hash);
} // fela
