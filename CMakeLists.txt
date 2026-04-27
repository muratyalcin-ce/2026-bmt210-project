cmake_minimum_required(VERSION 3.16)
project(MyDraw C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

# Option to use bundled raylib
option(USE_EXTERNAL_RAYLIB "Use raylib from external folder" ON)

if(USE_EXTERNAL_RAYLIB)
    add_subdirectory(external/raylib)
else()
    find_package(raylib REQUIRED)
endif()

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        include
)

# Link raylib
if(USE_EXTERNAL_RAYLIB)
    target_link_libraries(${PROJECT_NAME} raylib)
else()
    target_link_libraries(${PROJECT_NAME} raylib)
endif()

# Platform-specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        winmm
        gdi32
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}
        m
        pthread
        dl
        rt
        X11
    )
endif()

# Copy assets to build folder
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
