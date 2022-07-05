cmake_minimum_required(VERSION 3.22)
project(demon_keylogger C)

set(CMAKE_C_STANDARD 99)

add_executable(demon_keylogger main.c find_event_file.c find_event_file.h)
