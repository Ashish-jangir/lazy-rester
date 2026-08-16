cmake_minimum_required(VERSION 4.3.2)
project(
    LazyRESTer
  VERSION 0.1
  LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Log C++ Compiler information
message("C++ Compiler: ${CMAKE_CXX_COMPILER}")
message("C++ Compiler Id: ${CMAKE_CXX_COMPILER_ID}")
message("C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

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
  src/storage/state_store.cpp
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
target_link_libraries(
    LazyRESTer
  PRIVATE
  CURL::libcurl
  ftxui::ftxui
  nlohmann_json::nlohmann_json
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
