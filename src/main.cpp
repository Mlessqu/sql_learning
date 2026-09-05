#include<pqxx/pqxx>

#include "AuthorizationService.h++"
#include "DataBase.h++"
#include"DataStructs.h++"
//localhost:5432
//127.0.0.1:5432 - SQL server

//we simulate http requests with cli args for now



constexpr int min_arg_count = 2;
constexpr int max_arg_count = 5;
constexpr int cmd_arg_idx = 1;
constexpr int user_arg_idx = 2;
constexpr int pass_arg_idx = 3;
constexpr int token_arg_idx = 4;

std::optional<fela::ParsedData> parse_cli(int _arg_count, char** _arg_values)
{
    if (_arg_count < min_arg_count || _arg_count > max_arg_count)
    {
        return std::nullopt;
    }

    fela::ParsedData parsed_data{};
    //create acc 0, log in 1, log out 2
    for (int i = 1; i < _arg_count; ++i)
    {
        if (i == cmd_arg_idx)
        {
            try
            {
                int command = std::stoi(_arg_values[i]);
                parsed_data.command_ = static_cast<fela::CliCommand>(command);
            }
            catch (...)
            {
                return std::nullopt;
            }
        }
        if (i == user_arg_idx)
        {
            parsed_data.username_ = _arg_values[i];
        }
        if (i == pass_arg_idx)
        {
            parsed_data.password_ = _arg_values[i];
        }
        if (i == token_arg_idx)
        {
            parsed_data.hash_token_ = _arg_values[i];
        }
    }
    return parsed_data;
}


int main(int _arg_count, char** _arg_values)
{
    std::optional<fela::ParsedData> http_request = parse_cli(_arg_count,_arg_values);
    if (!http_request)
    {
        return 1;
    }
    std::string postgres_uri = "postgresql://postgres:mleko1235@localhost:5432/auth_service";
    fela::DataBase data_base(postgres_uri);
    fela::AuthorizationService authorization_service(data_base);
    authorization_service.handle_request(*http_request);
    return 0;
}
