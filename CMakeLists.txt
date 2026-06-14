cmake_minimum_required(VERSION 4.3.2)
project(
    LazyRESTer
  VERSION 0.1
  LANGUAGES CXX
)

add_executable(
    LazyRESTer
  src/main.cpp
)

target_include_directories(
    LazyRESTer
  PRIVATE
  include
)
include(FetchContent)

FetchContent_Declare(
  curl
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG curl-8_20_0
)
FetchContent_MakeAvailable(curl)
# first run this command if you are in debian:sudo apt install libcurl4-openssl-dev
# find_package(CURL REQUIRED)

target_link_libraries(
    LazyRESTer
  PRIVATE
  CURL::libcurl
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
