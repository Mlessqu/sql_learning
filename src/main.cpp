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



int main(int _arg_count, char** _arg_values)
{


    std::string postgres_uri = "postgresql://postgres:mleko1235@localhost:5432/auth_service";
    fela::DataBase data_base(postgres_uri);
    fela::AuthorizationService authorization_service(data_base);

    return 0;
}
