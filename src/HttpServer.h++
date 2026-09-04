#pragma once
/*
 * Responsibility:
 * listen to client requests over encrypted tls,
 * validate client inputs
 * talks to authorization service, and decryption maybe? I am not yet sure if I should decrypt here or within authorization service

 */
namespace fela
{
    class HttpServer
    {
    public:
        //TODO: we completely ignore http server for now and simulate requests with cli args
        HttpServer();
        void login_request();//stub
        void create_account_request();

    };
} // fela
