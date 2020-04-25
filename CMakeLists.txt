cmake_minimum_required(VERSION 3.12)

project(pingpong LANGUAGES CXX)

add_executable(pingpong pingpong.cc)

install(TARGETS pingpong DESTINATION .)
install(FILES run.sh DESTINATION .)
