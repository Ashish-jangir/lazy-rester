cmake_minimum_required(VERSION 4.3.2)
project(
    LazyRESTer
  VERSION 0.1
  LANGUAGES C CXX
  # since sqlite3 is a c library it requires c compiler to compile.
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    LazyRESTer
  src/main.cpp
  src/ui/main_view.cpp
  src/ui/request_explorer.cpp
  src/ui/editor.cpp
  src/ui/response_view.cpp
  src/networking/curl_http_client.cpp
  src/networking/http_request.cpp
  src/storage/state_store.cpp
  src/storage/database_store.cpp
  src/import/importer.cpp
  src/app/application.cpp
  src/logger/file_logger.cpp
  src/utils/utility.cpp
)

target_compile_definitions(
    LazyRESTer
  PRIVATE
  LAZY_RESTER_LEVEL_INFO
)

target_include_directories(
    LazyRESTer
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
include(FetchContent)
# lint_cmake: -readability/mixedcase, -readability/wonkycase
FetchContent_Declare(
  curl
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG curl-8_21_0
)
FetchContent_MakeAvailable(curl)
# first run this command if you are in debian:
# sudo apt install libcurl4-openssl-dev
# find_package(CURL REQUIRED)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v7.0.1
)
FetchContent_MakeAvailable(ftxui)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    sqlite3
    URL https://www.sqlite.org/2026/sqlite-amalgamation-3530400.zip
)
FetchContent_MakeAvailable(sqlite3)
# sqlite amalgamation is a single c file, so we can just compile it as a
# static library and link it to our project.
add_library(sqlite3_lib STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
target_include_directories(sqlite3_lib PUBLIC ${sqlite3_SOURCE_DIR})
#  sqlite3 is a thread-safe library, but we need to define
#  SQLITE_THREADSAFE=1 to enable it.
target_compile_definitions(sqlite3_lib PRIVATE SQLITE_THREADSAFE=1)
target_link_libraries(
    LazyRESTer
  PRIVATE
  CURL::libcurl
  ftxui::ftxui
  nlohmann_json::nlohmann_json
  sqlite3_lib
)
# Only should be used in development
target_compile_options(
    LazyRESTer
  PRIVATE
  -Wall
  -Wextra
  -Wpedantic
)
set(CMAKE_BUILD_TYPE Debug)
