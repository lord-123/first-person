cmake_minimum_required(VERSION 3.6)
project(first-person)

include_directories(${PROJECT_SOURCE_DIR}/src)

find_package(SFML 2.5.1 COMPONENTS system graphics REQUIRED)

set(CMAKE_VS_JUST_MY_CODE_DEBUGGING true)

add_executable(first-person src/main.cpp)

target_link_libraries(first-person sfml-graphics sfml-system)

file(GLOB RESOURCES "resources/*")

file(COPY ${RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)

file(GLOB SHADERS "src/shaders/*")

file(COPY ${SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders)