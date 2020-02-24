cmake_minimum_required(VERSION 3.6)
project(first-person)

include_directories(${PROJECT_SOURCE_DIR}/src)

find_package(SFML 2.5.1 COMPONENTS system graphics REQUIRED)

set(CMAKE_VS_JUST_MY_CODE_DEBUGGING true)

add_executable(first-person src/main.cpp)

target_link_libraries(first-person sfml-graphics sfml-system)

add_custom_target(copy-resource-directory ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		    ${CMAKE_SOURCE_DIR}/resources
			${CMAKE_CURRENT_BINARY_DIR}/resources
	COMMENT "Copying resource files to build tree" VERBATIM
)

add_dependencies(first-person copy-resource-directory)

add_custom_target(copy-shader-directory ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		    ${CMAKE_SOURCE_DIR}/src/shaders
			${CMAKE_CURRENT_BINARY_DIR}/shaders
	COMMENT "Copying shader files to build tree" VERBATIM
)

add_dependencies(first-person copy-shader-directory)