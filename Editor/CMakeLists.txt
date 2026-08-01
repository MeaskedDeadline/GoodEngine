cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE GOOD_EDITOR_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE GOOD_EDITOR_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(GoodEditor
        ${GOOD_EDITOR_SOURCES}
        ${GOOD_EDITOR_HEADERS}
)

target_include_directories(GoodEditor
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Engine
)

target_link_libraries(GoodEditor
        PRIVATE
        GoodEngine
)

if(WIN32)
    target_link_libraries(GoodEditor PRIVATE
            opengl32 gdi32 user32 kernel32 winmm ws2_32
    )
endif()

if(MSVC)
    target_compile_options(GoodEditor PRIVATE /W4)
else()
    target_compile_options(GoodEditor PRIVATE
            -Wall -Wextra -Wno-unused-parameter
    )
endif()

set_target_properties(GoodEditor PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)