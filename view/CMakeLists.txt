add_executable(pacman
        src/main.cpp
        src/Game.cpp
        src/StateManager.cpp
        src/LevelState.cpp
)

target_include_directories(pacman PUBLIC include)

target_link_libraries(pacman
        logic
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)
