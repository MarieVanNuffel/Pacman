add_executable(pacman
        src/main.cpp
        src/Game.cpp
        src/StateManager.cpp
        src/LevelState.cpp
        src/EntityView.cpp
        src/Camera.cpp
        src/CoinView.cpp
        src/FruitView.cpp
        src/GhostView.cpp
        src/PacmanView.cpp
        src/MazeView.cpp
        src/MenuState.cpp
        src/ConcreteEntityFactory.cpp
        src/GhostDoorView.cpp
)

target_include_directories(pacman PUBLIC include)

target_link_libraries(pacman
        logic
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
        DESTINATION ${CMAKE_BINARY_DIR})

