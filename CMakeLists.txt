cmake_minimum_required(VERSION 2.8.9)
project(conwatch)

file(GLOB SOURCES "Source/*.cpp")

add_executable(conwatch ${SOURCES})
