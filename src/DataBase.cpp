#include "DataBase.h++"

namespace fela
{
    //implementation details here
    DataBase::DataBase(std::string _connection_string) : connection_(_connection_string)
    {
    }


    DbResult DataBase::create_account(std::string _username, std::string _pass_hash)
    {
        return DbResult{.status_ = DatabaseStatus::ok};
    }


    DbResult DataBase::log_in_request(std::string _username)
    {
        return DbResult{.status_ = DatabaseStatus::ok};
    }


    DbResult DataBase::create_session(int _acc_id, std::string _pass_hash)
    {
        return DbResult{.status_ = DatabaseStatus::ok};
    }


    DbResult DataBase::validate_session(std::string _client_token)
    {
        return DbResult{.status_ = DatabaseStatus::ok};
    }


    DbResult DataBase::log_out(std::string _client_token)
    {
        return DbResult{.status_ = DatabaseStatus::ok};
    }
} // fela