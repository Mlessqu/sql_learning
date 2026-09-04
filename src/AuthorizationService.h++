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
        AuthorizationService();
        void handle_request();

    };
} // fela
