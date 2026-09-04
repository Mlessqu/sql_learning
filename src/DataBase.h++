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

        [[nodiscard]] DbResult create_account(std::string _username, std::string _pass_hash);
        [[nodiscard]] DbResult log_in_request(std::string _username);
        [[nodiscard]] DbResult create_session(int _account_id, std::string _pass_hash);



        [[nodiscard]] DbResult validate_session(std::string _client_token);
        [[nodiscard]] DbResult log_out(std::string _client_token);
    private:
        post_gres::connection connection_;
    };
} // fela
