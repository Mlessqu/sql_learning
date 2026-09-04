#include<pqxx/pqxx>

#include "DataBase.h++"
#include"DataStructs.h++"
//localhost:5432
//127.0.0.1:5432 - SQL server

//we simulate http requests with cli args for now



std::optional<fela::ParsedData> parse_cli(int _arg_count, char** _arg_values)
{
    std::optional<fela::ParsedData> parsed_data = std::nullopt;
    if (_arg_count > 4)
    {
        return std::nullopt;
    }
    if (_arg_count==0)
    {
        return std::nullopt;
    }
    //create acc 0, log in 1, log out 2
    for (int i = 0; i < _arg_count; ++i)
    {
        if (i == 0)
        {
            int command = std::stoi(_arg_values[i]);
            parsed_data->command_ = static_cast<fela::CliCommand>(command);
        }
        if (i == 1)
        {
            parsed_data->username_ = _arg_values[i];
        }
        if (i == 2)
        {
            parsed_data->password_ = _arg_values[i];
        }
        if (i == 3)
        {
            parsed_data->hash_token_ = _arg_values[i];
        }
        return parsed_data;
    }
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
    return 0;
}
