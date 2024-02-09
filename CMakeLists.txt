cmake_minimum_required(VERSION 3.28.2)

project(audio)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

set(OPENAL_LIBRARY debug "${CMAKE_SOURCE_DIR}/external/openal-soft/build/Debug")

set(OPENAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/openal-soft/include")

list(APPEND CMAKE_PREFIX_PATH ${OPENAL_LIBRARY})


file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE INCLUDE ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(audio ${SOURCE} ${INCLUDE})


target_include_directories(audio PRIVATE source)

target_include_directories(audio PRIVATE include)


target_link_libraries(audio ${OPENAL_LIBRARY})

target_include_directories(audio PUBLIC ${OPENAL_INCLUDE_DIR})