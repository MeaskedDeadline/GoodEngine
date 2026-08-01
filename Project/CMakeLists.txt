cmake_minimum_required(VERSION 3.20)
project(GoodProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GoodProject main.cpp)

target_include_directories(GoodProject
        PRIVATE
        ${CMAKE_SOURCE_DIR}/Engine
)

target_link_libraries(GoodProject
        PRIVATE
        GoodEngine
)

if(WIN32)
    target_link_libraries(GoodProject PRIVATE
            opengl32 gdi32 user32 kernel32 winmm
    )
endif()

set_target_properties(GoodProject PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)