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
    class PacmanView : public EntityView, public logic::Observer {
    public:
        explicit PacmanView(std::shared_ptr<logic::PacManModel> m);
        void draw(sf::RenderWindow& win, const Camera& cam) override;
        void updateSprite(double dt) override;

        void startDeath(); // start de death animatie
        void stopDeath(); // stop de death animatie
        double getDeathDuration() const; // duur death animatie

        void onNotify(int event) override;
    private:
        // animatie
        double animTimer = 0.0;
        double frameTime = 0.15;

        // sprite sheet frames
        static constexpr int FRAME_SIZE = 15;
        static constexpr int NUM_FRAMES = 2;
        int frameIndex = 0;
        int frames[NUM_FRAMES] = {0, 15}; // kolommen voor open, half-open, bijna dicht

        // death animatie
        bool deathPlaying = false;
        int deathFrameIndex = 0;
        static constexpr int NUM_DEATH_FRAMES = 12; // 12 frames voor de animatie

        // richting van Pac-Man
        std::weak_ptr<logic::PacManModel> model;
        sf::Sprite sprite;
        sf::Texture texture;

        logic::Direction lastDirection = logic::Direction::RIGHT;

    };
}

#endif //PACMANGAME_PACMANVIEW_H