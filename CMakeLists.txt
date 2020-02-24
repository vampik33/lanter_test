cmake_minimum_required(VERSION 3.2)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json json)

set(PROJECT_NAME bowling)
project(${PROJECT_NAME} VERSION 0.0.1)

set(SOURCES 
  main.cpp
  frame.cpp
)

set(HEADERS 
  frame.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries( ${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json )
