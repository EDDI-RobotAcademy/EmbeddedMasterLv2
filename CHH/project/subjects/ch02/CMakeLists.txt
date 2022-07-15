cmake_minimum_required(VERSION 3.0.0)
project(file_paser VERSION 0.1.0)

include(CTest)
enable_testing()

set(SRC
    ${CMAKE_SOURCE_DIR}/src/file_manager.c
    ${CMAKE_SOURCE_DIR}/src/queue.c

)
add_executable(file_paser main.c ${SRC})

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
