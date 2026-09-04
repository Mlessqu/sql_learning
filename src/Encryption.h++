#pragma once
#include <string>

//bundles helper functions that encrypts data
namespace fela::encryption
{
//TODO:: for now it's just dummy functions returning same string
    std::string hash_data(const std::string& _string) {return _string;}
} // fela
