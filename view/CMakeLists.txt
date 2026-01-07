cmake_minimum_required(VERSION 3.16)
project(PacMan LANGUAGES CXX)

add_executable(pacman
        src/main.cpp
        src/Game.cpp
        src/StateManager.cpp
        src/LevelState.cpp
        src/Camera.cpp
        src/CoinView.cpp
        src/FruitView.cpp
        src/GhostView.cpp
        src/PacmanView.cpp
        src/MazeView.cpp
        src/MenuState.cpp
        src/ConcreteEntityFactory.cpp
        src/GhostDoorView.cpp
        src/Resource.cpp
        src/PauseState.cpp
)

target_include_directories(pacman PUBLIC include)

target_link_libraries(pacman
        logic
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)

# Assets kopiëren naar de build map (indien aanwezig)
set(ASSETS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/assets")
if(EXISTS "${ASSETS_SRC}")
    # Kopieer de hele assets map naar de build root, zodat runtime path 'build/assets/...' werkt
    file(COPY "${ASSETS_SRC}" DESTINATION "${CMAKE_BINARY_DIR}")
else()
    message(WARNING "Assets directory not found at ${ASSETS_SRC}; skipping asset copy.")
endif()

