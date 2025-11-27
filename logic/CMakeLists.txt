add_library(logic
        src/World.cpp
        src/Stopwatch.cpp
        src/Random.cpp
)

target_include_directories(logic PUBLIC include)
set_target_properties(logic PROPERTIES CXX_STANDARD 17)
