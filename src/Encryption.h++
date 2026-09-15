#pragma once
#include <optional>
#include <string>

//talks to: AuthorizationService,
//bundles helper functions that encrypts data
namespace fela::encryption
{

    /**
     * @return unique hash token to manage active sessions
     */
    std::string generate_token();
    std::string encrypt_password(const std::string& _password);
    bool verify_password(const std::string& _password, const std::string& _stored_hash);
} // fela
