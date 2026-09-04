#include "DataBase.h++"

namespace fela
{
    //implementation details here
    DataBase::DataBase(std::string _connection_string)
    {
    }


    DbResult DataBase::create_account(std::string _username, std::string _pass_hash)
    {
    }


    DbResult DataBase::log_in_request(std::string _username)
    {
    }


    DbResult DataBase::create_session(int _account_id, std::string _pass_hash)
    {
    }


    DbResult DataBase::validate_session(std::string _client_token)
    {
    }


    DbResult DataBase::log_out(std::string _client_token)
    {
    }
} // fela