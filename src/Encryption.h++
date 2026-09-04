#pragma once
#include <string>
//talks to: AuthorizationService, HttpServer
//bundles helper functions that encrypts data
namespace fela::encryption
{
//TODO:: for now it's just function returning same string back
    std::string hash_data(const std::string& _string);
} // fela
