//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_PACMANVIEW_H
#define PACMANGAME_PACMANVIEW_H

#pragma once

#include "EntityView.h"
#include "Camera.h"
#include "logic/Direction.h"
#include "logic/Observer.h"

namespace logic {
    class PacManModel;
}

namespace view {

    /**
     * @brief View voor de ghosts
     *
     * Pacmanview tekent pacman in de juiste richting met zijn bijbehorende animaties.
     */
    class PacmanView : public EntityView, public logic::Observer {
    public:

        /**
         * @brief Constructor
         * @param m PacmanModel
         */
        explicit PacmanView(std::shared_ptr<logic::PacManModel> m);

        /**
         * @brief Tekent de ghosts
         */
        void draw(sf::RenderWindow& win, const Camera& cam) override;

        /**
         * @brief Update van animatie
         */
        void updateSprite(double dt) override;

        /**
         * @brief Start de death animatie
         */
        void startDeath();

        /**
         * @brief Stopt de death animatie
         */
        void stopDeath();

        /**
        * @brief Geeft de duur van de death animatie terug
        *
        * @return double duratie van de animatie.
        */
        [[nodiscard]] double getDeathDuration() const;

        /**
         * @brief Ontvangt events van de observer.
         */
        void onNotify(int event) override;
    private:
        // Animatie
        double animTimer = 0.0;
        double frameTime = 0.15;

        // Sprite sheet frames
        static constexpr int FRAME_SIZE = 15;
        static constexpr int NUM_FRAMES = 2;
        int frameIndex = 0;
        int frames[NUM_FRAMES] = {0, 15}; // kolommen voor open, half-open, bijna dicht

        // death animatie
        bool deathPlaying = false;
        int deathFrameIndex = 0;
        static constexpr int NUM_DEATH_FRAMES = 12; // 12 frames voor de animatie

        // pacman
        std::weak_ptr<logic::PacManModel> model;
        sf::Sprite sprite;
        std::shared_ptr<sf::Texture> texturePtr;

        logic::Direction lastDirection = logic::Direction::RIGHT;

    };
}

#endif //PACMANGAME_PACMANVIEW_H