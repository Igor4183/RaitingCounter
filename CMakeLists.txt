cmake_minimum_required(VERSION 3.20)

project(app LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE CPP_SRC CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE C_SRC CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

add_executable(app
    ${CPP_SRC}
    ${C_SRC}
)

target_include_directories(app PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/generator"
    "${CMAKE_SOURCE_DIR}/include/parser"
)

target_compile_options(app PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -mmacosx-version-min=26.0>
    $<$<COMPILE_LANGUAGE:C>:-Wall -Wextra -mmacosx-version-min=26.0>
)

set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)