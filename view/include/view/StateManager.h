//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_STATEMANAGER_H
#define PACMAN_STATEMANAGER_H

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "logic/Score.h"

class State;

/**
 * @brief Beheert de verschillende states van het spel.
 *
 * De StateManager houdt een stack bij van de states (menustate, levelstate, pausestate)
 * De bovenste state is de actieve state.
 *
 * Er is 1 score zodat de score behouden blijft bij statewissels.
 */
class StateManager {
public:
    /**
     * @brief Constructor
     * @param win Referentie naar het SFML renderwindow
     */
    StateManager(sf::RenderWindow& win);

    /**
     * @brief Voegt een nieuwe state toe bovenop de stack.
     *
     * De huidige state blijft bestaan, maar wordt gepauzeerd.
     *
     * @param state De nieuwe state die actief wordt
     */
    void pushState(std::shared_ptr<State> state);

    /**
     * @brief Verwijdert de huidige state van de stack.
     *
     * De vorige state wordt opnieuw actief als die bestaat.
     */
    void popState();

    /**
     * @brief Vervangt alle huidige states door één nieuwe state.
     *
     * Wordt gebruikt bij de overgangen, zoals:
     * - van menu naar level
     * - van game over terug naar menu
     *
     * @param state De nieuwe actieve state
     */
    void changeState(std::shared_ptr<State> state);

    /**
     * @brief Geeft input door aan de actieve state.
     *
     * @param ev SFML event
     */
    void handleInput(sf::Event& ev);

    /**
     * @brief Update de actieve state.
     *
     * @param dt Delta time (tijd sinds vorige frame)
     */
    void update(double dt);

    /**
     * @brief Rendert de actieve state.
     */
    void render();

    /**
     * @brief Geeft toegang tot de score.
     *
     * @return std::shared_ptr naar Score
     */
    [[nodiscard]] std::shared_ptr<logic::Score> getScore() const { return score; }

private:
    sf::RenderWindow& window;
    std::vector<std::shared_ptr<State>> states;
    std::shared_ptr<logic::Score> score;
};

#endif
