#include "DataBase.h++"
#include<iostream>
#include<pqxx/pqxx>
/*
 * Only ever talks to authorization service requests
 *List of postgres sql requests I will have to make:
 * 1. Account creation request: insert into accounts (username, password_hash) values($1,$2) returning id;
 * 2. Log in request: select id, password_hash from accounts where username=$1
 * 3. Create sessions: insert into sessions (account_id, token_hash, expiration_date) values($1, generated_32_hex_hash_token, now() + interval '7 days')
 * 4. Validate sessions: select account_id from sessions where token_hash=$1 and expiration_date > now();
 * 5. Invalidate sessions(logout): delete from sessions where token_hash=$1
 */
namespace fela
{
    //implementation details here
    DataBase::DataBase(std::string _connection_string) : connection_(_connection_string)
    {
    }


    DbResult DataBase::create_account(std::string _username, std::string _pass_hash)
    {
        pqxx::work create_acc_work{connection_};
        pqxx::params params{_username, _pass_hash};
        DbResult db_result{};
        try
        {
            pqxx::field result = create_acc_work.exec(
                "insert into accounts (username, password_hash) values($1,$2) returning id", params).one_field();
            db_result.acc_id_ = result.as<int>();
            create_acc_work.commit();
            db_result.status_ = DatabaseStatus::ok;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            db_result.status_ = DatabaseStatus::db_error;
        }
        return db_result;
    }


    DbResult DataBase::log_in_request(std::string _username)
    {
        pqxx::work log_in_work{connection_};
        pqxx::params params{_username};
        DbResult result;
        try
        {
            auto log_in_result = log_in_work.exec("select id, password_hash from accounts where username = $1", params).
                                             one_row();
            std::tuple<int, std::string> tuple = log_in_result.as<int, std::string>();
            result.status_ = DatabaseStatus::ok;
            result.acc_id_ = get<int>(tuple);
            result.acc_pass_hash_ = get<std::string>(tuple);
            log_in_work.commit();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            result.status_ = DatabaseStatus::db_error;
        }
        return result;
    }


    DbResult DataBase::create_session(int _acc_id, std::string _created_token)
    {
        pqxx::work create_session_work{connection_};
        pqxx::params params{_acc_id, _created_token};
        DbResult result;
        try
        {
            auto query = create_session_work.exec("insert into sessions(account_id, token_hash, expiration_date)"
                                                  " values($1,$2,now() + interval '5 mins') returning token_hash",
                                                  params).one_field();
            result.status_ = DatabaseStatus::ok;
            result.session_token_ = query.as<std::string>();
            create_session_work.commit();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << "\n";
            result.status_ = DatabaseStatus::db_error;
        }
        return result;
    }


    DbResult DataBase::validate_token_session(std::string _client_token)
    {
        pqxx::work validate_session_work{connection_};
        pqxx::params params{_client_token};
        DbResult result;
        try
        {
            auto query = validate_session_work.exec(
                "select account_id from sessions where token_hash = $1 and expiration_date > now()", params);
            result.status_ = DatabaseStatus::ok;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            result.status_ = DatabaseStatus::db_error;
        }
    }


    DbResult DataBase::log_out(std::string _client_token)
    {
        pqxx::work create_log_out_work{connection_};
        pqxx::params params{_client_token};
        DbResult result;
        try
        {
            auto query = create_log_out_work.exec(
                "delete from sessions where token_hash = $1", params);
            if (query.affected_rows()==1)
            {
                result.status_ = DatabaseStatus::ok;
            }else
            {
                result.status_ = DatabaseStatus::not_found;
            }
            create_log_out_work.commit();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << "\n";
            result.status_ = DatabaseStatus::db_error;
        }
        return result;
    }
} // fela
