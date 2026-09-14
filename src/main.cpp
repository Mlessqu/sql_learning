#include<pqxx/pqxx>

#include "AuthorizationService.h++"
#include "DataBase.h++"
#include"DataStructs.h++"
//localhost:5432
//127.0.0.1:5432 - SQL server

//we simulate http requests with cli args for now



constexpr int MIN_ARG_COUNT = 2;
constexpr int MAX_ARG_COUNT = 5;
constexpr int CMD_ARG_IDX = 1;
constexpr int USER_ARG_IDX = 2;
constexpr int PASS_ARG_IDX = 3;
constexpr int TOKEN_ARG_IDX = 4;

std::optional<fela::ParsedData> parse_cli(int _arg_count, char** _arg_values)
{
    if (_arg_count < MIN_ARG_COUNT || _arg_count > MAX_ARG_COUNT)
    {
        return std::nullopt;
    }

    fela::ParsedData parsed_data{};

    for (int i = 1; i < _arg_count; ++i)
    {
        if (i == CMD_ARG_IDX)
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
        if (i == USER_ARG_IDX)
        {
            parsed_data.username_ = _arg_values[i];
        }
        if (i == PASS_ARG_IDX)
        {
            parsed_data.password_ = _arg_values[i];
        }
        if (i == TOKEN_ARG_IDX)
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

    return 0;
}
