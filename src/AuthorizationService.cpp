#include "AuthorizationService.h++"

#include <iostream>

#include"Encryption.h++"
namespace fela
{
    AuthorizationService::AuthorizationService(DataBase& _data_base) : data_base_(_data_base)
    {
    }


    bool AuthorizationService::create_account(const std::string& _username, const std::string& _password)
    {
        auto result= data_base_.create_account(_username,encryption::encrypt_password(_password));
        if (result.status_ == DatabaseStatus::ok)
        {
                return true;
        }
        return false;
    }


    std::optional<std::string> AuthorizationService::log_in(const std::string& _username, const std::string& _password)
    {
        DbResult db_result = data_base_.log_in_request(_username);
        if (!db_result.acc_id_)
        {
            return std::nullopt;
        }
        int acc_id = *db_result.acc_id_;
        if (!db_result.acc_pass_hash_)
        {
            return std::nullopt;
        }

        if (encryption::verify_password(_password, *db_result.acc_pass_hash_))
        {
            auto created_token = encryption::generate_token();
            auto result = data_base_.create_session(acc_id, created_token);
            std::cout << "Logged in!, here's your cookie:" << created_token;
            return created_token;
        }
        return std::nullopt;
    }


    bool AuthorizationService::log_out(const std::string& _token)
    {

        auto result = data_base_.log_out(_token);
        if (result.status_ != DatabaseStatus::ok)
        {
            std::cout << "eh? who are you?";
            return false;
        }
        std::cout << "ok, bye! logged out";
        return true;
    }


    //implementation details here
} // fela
