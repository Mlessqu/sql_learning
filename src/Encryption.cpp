#include "Encryption.h++"
#include<openssl/evp.h>
#include<openssl/rand.h>
#include<format>
namespace fela::encryption
{
    constexpr int TOKEN_BYTE_LEN = 32;
    constexpr int SALT_LEN = 16;
    constexpr int HASH_LEN = 32;
    constexpr int PBKDF2_ITERATIONS = 100'000;

    std::string hash_data(const std::string& _string)
    {
        return _string + "hashed";
    }


    bool verify_password(const std::string& _password, const std::string& _stored_hash)
    {
        auto delimiter_pos = _stored_hash.find(':');
        if (delimiter_pos == std::string::npos)
        {
            return false;
        }

        std::string salt_hex = _stored_hash.substr(0, delimiter_pos);
        std::string expected_hash_hex = _stored_hash.substr(delimiter_pos + 1);

        constexpr size_t hex_salt_len = SALT_LEN * 2;
        if (salt_hex.length() != hex_salt_len)
        {
            return false;
        }

        unsigned char salt[SALT_LEN];
        for (size_t i = 0; i < SALT_LEN; ++i)
        {
            salt[i] = static_cast<unsigned char>(std::stoi(salt_hex.substr(i * 2, 2), nullptr, 16));
        }

        unsigned char computed_hash[HASH_LEN];
        PKCS5_PBKDF2_HMAC(_password.c_str(), static_cast<int>(_password.length()), salt, SALT_LEN, PBKDF2_ITERATIONS, EVP_sha256(), HASH_LEN, computed_hash);

        std::string computed_hash_hex;
        for (int i = 0; i < HASH_LEN; ++i)
        {
            computed_hash_hex += std::format("{:02x}", computed_hash[i]);
        }

        return computed_hash_hex == expected_hash_hex;
    }


    [[nodiscard]] std::string generate_token()
    {
        unsigned char buffer[TOKEN_BYTE_LEN];
        RAND_bytes(buffer, TOKEN_BYTE_LEN);

        std::string hex_str;
        for (size_t i = 0; i < TOKEN_BYTE_LEN; ++i)
        {
            hex_str += std::format("{:02x}", buffer[i]);
        }
        return hex_str;
    }

    std::string encrypt_password(const std::string& _password)
    {
        unsigned char salt[SALT_LEN];
        RAND_bytes(salt, SALT_LEN);

        unsigned char hash[HASH_LEN];
        PKCS5_PBKDF2_HMAC(_password.c_str(), static_cast<int>(_password.length()), salt, SALT_LEN, PBKDF2_ITERATIONS, EVP_sha256(), HASH_LEN, hash);
        std::string salt_hex;
        for (int i = 0; i < SALT_LEN; ++i)
        {
            salt_hex += std::format("{:02x}", salt[i]);
        }
        std::string hash_hex;
        for (int i = 0; i < HASH_LEN; ++i)
        {
            hash_hex += std::format("{:02x}", hash[i]);
        }
        return salt_hex + ":" + hash_hex;
    }


    //implementation details here
} // fela
