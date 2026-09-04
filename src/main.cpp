#include<pqxx/pqxx>
#include<iostream>
//localhost:5432
//127.0.0.1:5432 - SQL server
//Milestone 0.5: connected to server db and told it to return 1 to me and it did :O[x]
//Milestone 1: recreate file reading with server based table[x]
//Milestone 2: simple cli credentials on success, db sends back data, print data[x]

//milestone 3:
int main(int _arg_count, char** _arg_values)
{
    constexpr const char* server_addr = "postgresql://postgres:mleko1235@127.0.0.1:5432/auth_service";
    try
    {
        pqxx::connection cx{server_addr};

        pqxx::work tx{cx}; //transaction
        std::cout << "Enter username:";
        std::string username;
        std::cin >> username;
        std::cout << "Enter password:";
        std::string password;
        std::cin >> password;

        auto result= tx.query<std::string,std::string>("select * from public.userbase where username=$1",pqxx::params{tx,username});
        for (auto[username,password] : result)
        {
            std::cout << "Username:" << username << ", passwd:" << password << "\n";
        }
        tx.commit();

    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
