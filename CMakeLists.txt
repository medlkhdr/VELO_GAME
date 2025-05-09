cmake_minimum_required(VERSION 3.16)
project(BikeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BikeGame main.cpp)

# Find SFML installed via MSYS2 (UCRT64)
set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

target_link_libraries(BikeGame sfml-graphics sfml-window sfml-audio sfml-system)
