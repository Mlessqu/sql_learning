#pragma once
#include <string>
#include<pqxx/pqxx>
namespace post_gres = pqxx;
/*
 *List of postgres sql requests I will have to make:
 * 1. Account creation request: insert into accounts (username, password_hash) values($1,$2) returning id;
 * 2. Log in request: select id, password_hash from accounts where username=$1
 * 3. Create session: insert into session (account_id, token_hash, expiration_date) values($1, generated_32_hex_hash_token, now() + interval '7 days')
 * 4. Validate session: select account_id from session where token_hash=$1 and expiration_date > now();
 * 5. Invalidate session(logout): delete from session where token_hash=$1
 */
namespace fela
{
    class DataBase
    {
        DataBase();

        [[nodiscard]] post_gres::result create_account(std::string _username, std::string _pass_hash);
        [[nodiscard]] post_gres::result log_in_request(std::string _username);
        [[nodiscard]] post_gres::result create_session(int _account_id, std::string _pass_hash);



        [[nodiscard]] post_gres::result validate_session(std::string _client_token);
        [[nodiscard]] post_gres::result log_out(std::string _client_token);
    private:
        post_gres::connection connection_;
    };
} // fela
