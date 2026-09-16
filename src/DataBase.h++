#pragma once
#include <string>
#include<pqxx/pqxx>
#include"DataStructs.h++"
namespace post_gres = pqxx;
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

    class DataBase
    {
    public:
        DataBase(std::string _connection_string);

        //account management
        [[nodiscard]] DbResult insert_account(std::string _username, std::string _pass_encrypted);
        [[nodiscard]] DbResult find_account_by_username(std::string _username);
        [[nodiscard]] DbResult find_username_by_acc_id(int _id);
        [[nodiscard]] DbResult insert_session(int _acc_id, std::string _created_token);

        //---session management
        [[nodiscard]] DbResult find_session(std::string _client_token);
        [[nodiscard]] DbResult delete_session(std::string _client_token);
    private:
        post_gres::connection connection_;
    };
} // fela
