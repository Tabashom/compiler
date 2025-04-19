cmake_minimum_required(VERSION 3.4.1)

add_library(
        lexer
        SHARED
        lexer.c
)

find_library(
        log-lib
        log
)

target_link_libraries(
        lexer
        ${log-lib}
)
