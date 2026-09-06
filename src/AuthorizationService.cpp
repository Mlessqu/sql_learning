#include "AuthorizationService.h++"

#include <iostream>

#include"Encryption.h++"
namespace fela
{
    AuthorizationService::AuthorizationService(DataBase& _data_base) : data_base_(_data_base)
    {
    }


    bool AuthorizationService::handle_request(ParsedData _request_data)
    {
        switch (_request_data.command_)
        {
        case CliCommand::create_account:
            return create_account(_request_data);
        case CliCommand::log_in:
            return log_in(_request_data);
        case CliCommand::log_out:
            return log_out(_request_data);
        default:
            return false;
        }
    }


    bool AuthorizationService::create_account(ParsedData& _data)
    {

        //stub just passes data down, prepares needed variables,
        std::string username = *_data.username_;
        std::string pass_hash = encryption::hash_data(*_data.password_);
        auto result= data_base_.create_account(username,pass_hash);
        if (result.status_ == DatabaseStatus::ok)
        {
            return true;
        }
        return false;
    }


    bool AuthorizationService::log_in(ParsedData& _data)
    {
        std::string client_username = *_data.username_;
        std::string client_pass_hash = encryption::hash_data(*_data.password_);
        auto db_result = data_base_.log_in_request(client_username);
        int acc_id = *db_result.acc_id_;
        if (*db_result.acc_pass_hash_ == client_pass_hash)
        {
            auto created_token = encryption::hash_data("dummyvalue");
            auto result = data_base_.create_session(acc_id,created_token);
            std::cout << "Logged in!, here's your cookie:" << created_token;
            return true;
        }
        return false;
    }


    bool AuthorizationService::log_out(ParsedData& _data)
    {
        std::string username = *_data.username_;
        std::string hashed_token = *_data.hash_token_;
        auto result = data_base_.log_out(hashed_token);
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
