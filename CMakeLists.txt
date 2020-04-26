cmake_minimum_required(VERSION 3.12)

project(pingpong LANGUAGES CXX)

find_package(Threads)

add_executable(pingpong pingpong.cc)
target_link_libraries(pingpong PRIVATE Threads::Threads)

install(TARGETS pingpong DESTINATION .)
install(FILES run.sh DESTINATION .)
