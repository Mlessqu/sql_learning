
#include "AuthorizationService.h++"
#include "DataBase.h++"
#include "HttpServer.h++"
//localhost:5432
//127.0.0.1:5432 - SQL server

//we simulate http requests with cli args for now




int main(int _arg_count, char** _arg_values)
{


    std::string postgres_uri = "postgresql://postgres:mleko1235@localhost:5432/auth_service";
    fela::DataBase data_base(postgres_uri);
    fela::AuthorizationService authorization_service(data_base);
    fela::HttpServer http_server(authorization_service);
    http_server.start(6767);
    return 0;
}
