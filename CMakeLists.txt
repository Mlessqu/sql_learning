cmake_minimum_required(VERSION 4.3)
project(auth_service)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(libpqxx REQUIRED)

add_executable(auth_service src/main.cpp
        src/AuthorizationService.cpp
        src/AuthorizationService.h++
        src/Encryption.cpp
        src/Encryption.h++
        src/HttpServer.cpp
        src/HttpServer.h++
        src/DataBase.cpp
        src/DataBase.h++
        src/DataStructs.h++
)
target_link_libraries(auth_service PRIVATE libpqxx::pqxx)

