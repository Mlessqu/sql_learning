cmake_minimum_required(VERSION 4.3)
project(sql_learning)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(libpqxx REQUIRED)

add_executable(sql_learning src/main.cpp
        src/AuthorizationService.cpp
        src/AuthorizationService.h++
        src/Encryption.cpp
        src/Encryption.h++
        src/HttpServer.cpp
        src/HttpServer.h++
        src/DataBase.cpp
        src/DataBase.h++
)
target_link_libraries(sql_learning PRIVATE libpqxx::pqxx)

