cmake_minimum_required(VERSION 3.2)

project(Homework12.5)

add_executable(Homework12.5 main.cpp)

add_subdirectory("C:\\Users\\Kir\\Desktop\\Netology Homeworks\\libpqxx-7.8.1" libpqxx-build)

target_compile_features(Homework12.5 PRIVATE cxx_std_17)

target_link_libraries(Homework12.5 PRIVATE pqxx)
