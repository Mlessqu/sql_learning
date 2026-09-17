
#include <iostream>

#include "AuthorizationService.h++"
#include "Config.h++"
#include "DataBase.h++"
#include "HttpServer.h++"

int main(int _arg_count, char** _arg_values)
{
    auto config_opt = fela::load_config("fela.conf");
    if (!config_opt)
    {
        std::cerr << "Fatal: Failed to load configuration. Aborting.\n";
        return 1;
    }
    const auto& config = *config_opt;

    fela::DataBase data_base(config.db_uri_);
    fela::AuthorizationService authorization_service(data_base);
    fela::HttpServer http_server(authorization_service);

    std::cout << "Starting server on port " << config.port_ << "...\n";
    http_server.start(static_cast<int>(config.port_));
    return 0;
}

