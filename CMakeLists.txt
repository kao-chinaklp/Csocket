cmake_minimum_required(VERSION 3.23)

# set the project name
project(Csocket)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(EXPORT_COMPILE_COMMANDS ON)

# add the executable
file(GLOB SOURCES "src/*.cpp")
add_executable(Csocket ${SOURCES})

set(CMAKE_AR gcc-ar)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static-libstdc++ -static-libgcc")
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(Csocket PRIVATE winmm wsock32 ws2_32)
endif()

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    target_compile_options(Csocket PRIVATE ${CMAKE_CXX_FLAGS} -pthread -fdiagnostics-color=always -pedantic)
endif()
if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    target_compile_options(Csocket PRIVATE ${CMAKE_CXX_FLAGS} -pthread -fdiagnostics-color=always -pedantic -O3 -flto -s)
endif()

target_include_directories(Csocket PRIVATE ${CMAKE_SOURCE_DIR}/header)