#pragma once
/*
 * Responsibility:
 * listen to client requests over encrypted open ssl,
 * validate client inputs
 * talks to authorization service, and decryption maybe? I am not yet sure if I should decrypt here or within authorization service
 * TODO:
 *

 */
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include<httplib.h>
namespace fela
{
    class AuthorizationService;

    class HttpServer
    {
    public:
        HttpServer(AuthorizationService& _auth_service);
        httplib::Response login_request(std::string _username, std::string _password);
        httplib::Response logout_request(std::string _token);
        httplib::Response create_account_request(std::string _username, std::string _password);
        void start(int _port);
        void stop();
    private:
        // httplib::Server server_;
        httplib::SSLServer server_;
        //TODO: change to http with ssl later, no encryption for testing
        fela::AuthorizationService& auth_service_;
    };
} // fela
