cmake_minimum_required(VERSION 3.10)

project(pegtl "CXX")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

function(create_executable executable_name)
    add_executable(${executable_name} src/${executable_name}.cpp)
endfunction()

include_directories(include)

create_executable(example1)
create_executable(ini)
create_executable(multirules)
create_executable(list_of_numbers)
create_executable(json)

file(COPY inputs DESTINATION ${PROJECT_BINARY_DIR})