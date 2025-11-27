cmake_minimum_required(VERSION 3.16)
project(PacmanGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML zoeken
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

# Subdirectories toevoegen
add_subdirectory(logic)
add_subdirectory(view)
