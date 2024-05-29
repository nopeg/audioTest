cmake_minimum_required(VERSION 3.28.2)

project(audio)

#if(CMAKE_BUILD_TYPE STREQUAL "Release")
#	target_link_options(simple PRIVATE -mwindows)
#endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE INCLUDE ${PROJECT_SOURCE_DIR}/include/*.h)


set(OPENAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/openal-soft/include")

set(OPENAL_PATH "${CMAKE_SOURCE_DIR}/external/openal-soft/build")

list(APPEND CMAKE_PREFIX_PATH ${OPENAL_PATH})

find_package(OpenAL REQUIRED)


add_library(audio ${SOURCE} ${INCLUDE})

target_include_directories(audio PRIVATE source)

target_include_directories(audio PRIVATE include)

target_link_libraries(audio PUBLIC OpenAL::OpenAL)