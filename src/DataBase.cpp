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
    DataBase::DataBase(std::string _connection_string) : connection_(_connection_string)
    {
    }


    DbResult DataBase::insert_account(std::string _username, std::string _pass_encrypted)
    {
        pqxx::work insert_acc_work{connection_};
        pqxx::params params{_username, _pass_encrypted};
        DbResult db_result{};
        try
        {
            pqxx::field result = insert_acc_work.exec(
                "insert into accounts (username, password_hash) values($1,$2) returning id", params).one_field();
            db_result.acc_id_ = result.as<int>();
            insert_acc_work.commit();
            db_result.status_ = DatabaseStatus::ok;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            db_result.status_ = DatabaseStatus::db_error;
        }
        return db_result;
    }


    DbResult DataBase::find_account_by_username(std::string _username)
    {
        pqxx::work find_account_work{connection_};
        pqxx::params params{_username};
        DbResult result;
        try
        {
            auto log_in_result = find_account_work.exec("select id, password_hash from accounts where username = $1",
                                                        params).
                                                   one_row();
            std::tuple<int, std::string> tuple = log_in_result.as<int, std::string>();
            result.status_ = DatabaseStatus::ok;
            result.acc_id_ = get<int>(tuple);
            result.acc_pass_hash_ = get<std::string>(tuple);
            find_account_work.commit();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            result.status_ = DatabaseStatus::db_error;
        }
        return result;
    }


    DbResult DataBase::insert_session(int _acc_id, std::string _created_token)
    {
        pqxx::work insert_session_work{connection_};
        pqxx::params params{_acc_id, _created_token};
        DbResult result;
        try
        {
            auto query = insert_session_work.exec("insert into sessions(account_id, token_hash, expiration_date)"
                                                  " values($1,$2,now() + interval '5 mins') returning token_hash",
                                                  params).one_field();
            result.status_ = DatabaseStatus::ok;
            result.session_token_ = query.as<std::string>();
            insert_session_work.commit();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << "\n";
            result.status_ = DatabaseStatus::db_error;
        }
        return result;
    }


    DbResult DataBase::find_session(std::string _client_token)
    {
        pqxx::work find_session_work{connection_};
        pqxx::params params{_client_token};
        DbResult result;
        try
        {
            std::optional<int> query_result = std::nullopt;
            query_result = find_session_work.exec(
                "select account_id from sessions where token_hash = $1 and expiration_date > now()",
                params).one_field().as<int>();
            if (query_result)
            {
                result.acc_id_ = *query_result;
                result.status_ = DatabaseStatus::ok;
            }
            else
            {
                result.status_ = DatabaseStatus::not_found;
            }
            find_session_work.commit();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            result.status_ = DatabaseStatus::db_error;
        }
        return result;
    }


    DbResult DataBase::find_username_by_acc_id(int _id)
    {
        pqxx::work find_user_work{connection_};
        pqxx::params params{_id};
        DbResult result;
        try
        {
                std::optional query_result = find_user_work.exec("select username from accounts where id =$1").one_field().as<std::string>();
            result.acc_user_ = query_result;
            result.status_ = DatabaseStatus::ok;
            find_user_work.commit();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << "\n";
            result.status_ = DatabaseStatus::db_error;
        }
        return result;
    }


    DbResult DataBase::delete_session(std::string _client_token)
    {
        pqxx::work delete_session_work{connection_};
        pqxx::params params{_client_token};
        DbResult result;
        try
        {
            auto query = delete_session_work.exec(
                "delete from sessions where token_hash = $1", params);
            if (query.affected_rows() == 1)
            {
                result.status_ = DatabaseStatus::ok;
            }
            else
            {
                result.status_ = DatabaseStatus::not_found;
            }
            delete_session_work.commit();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << "\n";
            result.status_ = DatabaseStatus::db_error;
        }
        return result;
    }
} // fela
