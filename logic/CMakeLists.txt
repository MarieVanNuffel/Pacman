add_library(logic
        src/World.cpp
        src/Stopwatch.cpp
        src/Random.cpp
        src/PacManModel.cpp
        src/GhostModel.cpp
        src/CoinModel.cpp
        src/FruitModel.cpp
        src/Score.cpp
        src/Subject.cpp
        include/logic/Observer.h
        src/GhostDoorModel.cpp
        include/logic/Events.h

)

target_include_directories(logic PUBLIC include)
set_target_properties(logic PROPERTIES CXX_STANDARD 17)
