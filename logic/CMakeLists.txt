add_library(logic
        src/World.cpp
        src/Stopwatch.cpp
        src/Random.cpp
        src/Entity.cpp
        src/PacManModel.cpp
        src/GhostModel.cpp
        src/Coin.cpp
        src/Fruit.cpp
        src/IEntityFactory.cpp
        src/Score.cpp
)

target_include_directories(logic PUBLIC include)
set_target_properties(logic PROPERTIES CXX_STANDARD 17)
