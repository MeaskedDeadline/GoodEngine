cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE GOOD_ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE GOOD_ENGINE_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(GoodEngine STATIC
        ${GOOD_ENGINE_SOURCES}
        ${GOOD_ENGINE_HEADERS}
)

target_include_directories(GoodEngine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
    target_link_libraries(GoodEngine PUBLIC
            opengl32 gdi32 user32 kernel32 winmm
    )
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(GoodEngine PUBLIC OpenGL::GL X11 pthread dl)
elseif(APPLE)
    find_library(COCOA_LIB  Cocoa  REQUIRED)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(GoodEngine PUBLIC ${COCOA_LIB} ${OPENGL_LIB} pthread dl)
endif()

if(MSVC)
    target_compile_options(GoodEngine PRIVATE /W4)
else()
    target_compile_options(GoodEngine PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()