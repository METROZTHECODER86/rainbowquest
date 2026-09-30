cmake_minimum_required(VERSION 3.22)
include(qpm_defines.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(${COMPILE_ID})

add_compile_options(-frtti -fexceptions -fvisibility=hidden -fPIE -O3)
add_compile_definitions(VERSION=\"${MOD_VERSION}\")
add_compile_definitions(MOD_ID=\"${MOD_ID}\")

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

RECURSE_FILES(cpp_file_list ${SOURCE_DIR}/*.cpp)
RECURSE_FILES(c_file_list ${SOURCE_DIR}/*.c)

add_library(${COMPILE_ID} SHARED ${cpp_file_list} ${c_file_list})

target_include_directories(${COMPILE_ID} PRIVATE .)
target_include_directories(${COMPILE_ID} PRIVATE ${SOURCE_DIR})
target_include_directories(${COMPILE_ID} PRIVATE ${INCLUDE_DIR})
target_include_directories(${COMPILE_ID} PUBLIC ${SHARED_DIR})

target_link_libraries(${COMPILE_ID} PRIVATE -llog)

include(extern.cmake)
