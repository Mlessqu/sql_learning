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
        bool handle_request(ParsedData _http_request_data);
    private:
        bool create_account(ParsedData& _data);
        bool log_in(ParsedData& _data);
        bool log_out(ParsedData& _data);
        DataBase& data_base_;
    };
} // fela
