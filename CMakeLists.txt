cmake_minimum_required(VERSION 3.16)

project(CppTetrisGame LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR
        "This version uses the Win32 API and currently builds only on Windows. "
        "A portable core or cross-platform frontend can be added later.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB TETRIS_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(tetris WIN32 ${TETRIS_SOURCES})
set_target_properties(tetris PROPERTIES OUTPUT_NAME "main")

target_link_libraries(tetris PRIVATE gdi32 user32)

if(MSVC)
    target_compile_options(tetris PRIVATE /W4 /permissive-)
else()
    target_compile_options(tetris PRIVATE -Wall -Wextra)
endif()
