cmake_minimum_required(VERSION 3.6)
project(first-person)

include_directories(${PROJECT_SOURCE_DIR}/src)

find_package(SFML 2.5.1 COMPONENTS system graphics REQUIRED)

set(CMAKE_VS_JUST_MY_CODE_DEBUGGING true)

add_executable(first-person src/main.cpp)

target_link_libraries(first-person sfml-graphics sfml-system)

add_custom_command(
	TARGET first-person POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		    ${CMAKE_SOURCE_DIR}/resources
			${CMAKE_CURRENT_BINARY_DIR}/resources
)

add_custom_command(
	TARGET first-person POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		    ${CMAKE_SOURCE_DIR}/src/shaders
			${CMAKE_CURRENT_BINARY_DIR}/shaders
)