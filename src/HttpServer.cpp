#include "HttpServer.h++"

#include <filesystem>

#include "AuthorizationService.h++"

namespace fela
{
    constexpr int REQUEST_SUCCESS = 200;
    constexpr int REQUEST_FAILED = 400;
    constexpr int UNAUTHORIZED = 401;
    constexpr int DUPLICATE_USERNAME = 409;
    constexpr int REQUEST_NOT_FOUND = 404;

    const char* CERT_PATH = "/home/missqu/projects/sql_learning/cert.pem";
    const char* KEY_PATH = "/home/missqu/projects/sql_learning/key.pem";
    fela::HttpServer::HttpServer(AuthorizationService& _auth_service) : auth_service_(_auth_service), server_(CERT_PATH,KEY_PATH)
    {

        server_.Post("/register", [this](const httplib::Request& _request, httplib::Response& _response)
        {


            std::string user = _request.get_param_value("username");
            std::string pass = _request.get_param_value("password");
            _response = create_account_request(user, pass);
        });
        server_.Post("/login", [this](const httplib::Request& _request, httplib::Response& _response)
        {

            std::string username = _request.get_param_value("username");
            std::string password = _request.get_param_value("password");
            if (username.empty()||password.empty())
            {
             _response.set_content("Invalid request", "text/plain");
            }
            _response = this->login_request(username, password);

        });
        server_.Post("/logout", [this](const httplib::Request& _request, httplib::Response& _response)
        {
            std::string token = _request.get_header_value("Authorization");
            _response = this->logout_request(token);
        });
        server_.Post("/me",[this](const httplib::Request& _request,httplib::Response& _response)
        {

        });
    }


    httplib::Response fela::HttpServer::login_request(std::string _username, std::string _password)
    {
        httplib::Response response;
        std::optional<std::string> token = auth_service_.log_in(_username, _password);
        if (token)
        {
            response.status = REQUEST_SUCCESS;
            std::string response_value = "Login success, token= " + *token + "\n";

            response.set_content(response_value, "text/plain");
        }else
        {
            response.status = REQUEST_FAILED;
            response.set_content("Login failed", "text/plain");
        }
        return response;
    }


    httplib::Response HttpServer::request_username(std::string _token)
    {
        httplib::Response response;
        std::optional<std::string> request_result = auth_service_.session_username(_token);
        if (!request_result)
        {
            response.status = UNAUTHORIZED;
            return response;
        }
        response.status = REQUEST_SUCCESS;
        std::string response_value = "Your username is: ";
        response_value+= *request_result;
        response.set_content(response_value,"text/plain");
        return response;
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
        return response;
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


    void fela::HttpServer::start(int _port)
    {
        if (!server_.is_valid())
        {
            std::cerr << "Server failed to run, check fo correct path" << std::filesystem::current_path() << "\n";
            return;
        }
        constexpr char* listen_ip = "0.0.0.0";
        server_.listen(listen_ip, _port);
    }


    void fela::HttpServer::stop()
    {
        server_.stop();
    }
}
