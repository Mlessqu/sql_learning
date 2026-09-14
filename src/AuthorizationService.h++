#pragma once
#include"DataBase.h++"
/*
 * Responsibility: authorization logic, interpretting sql results
 * Talks  to: httpserver, encryption and database
 */
namespace fela
{
    class AuthorizationService
    {
    public:
        AuthorizationService(DataBase& _data_base);

        bool create_account(const std::string& _username, const std::string& _password);
        std::optional<std::string> log_in(const std::string& _username, const std::string& _password);
        bool log_out(const std::string& _token);
    private:
        DataBase& data_base_;
    };
} // fela
