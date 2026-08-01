cmake_minimum_required(VERSION 3.20)
project(GoodEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(WIN32)
    add_compile_definitions(GOOD_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(GOOD_PLATFORM_LINUX)
elseif(APPLE)
    add_compile_definitions(GOOD_PLATFORM_MACOS)
endif()

add_compile_definitions(
        $<$<CONFIG:Debug>:GOOD_DEBUG>
        $<$<CONFIG:Release>:GOOD_RELEASE>
)

add_subdirectory(Engine)
add_subdirectory(Editor)