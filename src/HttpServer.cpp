#include "HttpServer.h++"

#include "AuthorizationService.h++"

namespace fela
{
    constexpr int REQUEST_SUCCESS = 200;
    constexpr int REQUEST_FAILED = 400;


    fela::HttpServer::HttpServer(AuthorizationService& _auth_service) : auth_service_(_auth_service)
    {
        //init server here
        server_.Post("/register", [this](const httplib::Request& _request, httplib::Response& _response)
        {
            std::string user = _request.get_param_value("username");
            std::string pass = _request.get_param_value("password");
            auto response = create_account_request(user, pass);
            _response = response;

            //parse and return response into response args
        });
        server_.Post("/login", [this](const httplib::Request& _request, httplib::Response& _response)
        {
            std::string username = _request.get_param_value("username");
            std::string password = _request.get_param_value("password");
            auto cookie = this->login_request(username, password);
            _response = cookie;
        });
        server_.Post("/logout", [this](const httplib::Request& _request, httplib::Response& _response)
        {
            auto cookie = _request.get_param_value("cookie");
            auto result = this->logout_request(cookie);
        });
    }


    httplib::Response fela::HttpServer::login_request(std::string _username, std::string _password)
    {
        httplib::Response response;
        auto result = auth_service_.log_in(_username, _password);
        if (result)
        {

        }else
        {

        }
    }


    httplib::Response fela::HttpServer::logout_request(std::string _token)
    {
        httplib::Response response;
        bool result = auth_service_.log_out(_token);
        if (result)
        {
            response.status = REQUEST_SUCCESS;
            response.set_content("Logged out", "text/plain");
        }
        else
        {
            response.status = REQUEST_FAILED;
            response.set_content("Who are you again?", "text/plain");
        }
    }


    httplib::Response fela::HttpServer::create_account_request(std::string _username, std::string _password)
    {
        httplib::Response response;
        bool result = auth_service_.create_account(_username, _password);
        if (result)
        {
            response.status = REQUEST_SUCCESS;
            response.set_content("Account created.", "text/plain");
        }
        else
        {
            response.status = REQUEST_FAILED;
            response.set_content("Account creation failed.", "text/plain");
        }
        return response;
    }


    bool fela::HttpServer::start(int _port)
    {
        constexpr char* listen_ip = "0.0.0.0";
        server_.listen(listen_ip, _port);
    }


    void fela::HttpServer::stop()
    {
        server_.stop();
    }
}
