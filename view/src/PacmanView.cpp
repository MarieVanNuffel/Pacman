//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "logic/PacManModel.h"
#include "../include/view/PacmanView.h"
#include "logic/Events.h"
#include <iostream>
#include <SFML/Graphics.hpp>

namespace view {
    PacmanView::PacmanView(std::shared_ptr<logic::PacManModel> m)
        : EntityView(m), model(m)
    {
        if (!texture.loadFromFile("view/assets/pacman.png")) {
            throw std::runtime_error("Failed to load pacman texture!");
        }

        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_SIZE, FRAME_SIZE));
        sprite.setOrigin(FRAME_SIZE / 2.f, FRAME_SIZE / 2.f);
    }


    void PacmanView::draw(sf::RenderWindow& win, const Camera& cam)
    {
        double sizeW = 0.9;
        double sizeH = 0.9;

        sf::FloatRect rect = cam.worldToPixels(model->getX() - sizeW/2.0,
                                           model->getY() - sizeH/2.0,
                                           sizeW, sizeH);
        sprite.setPosition(rect.left + rect.width/2.f,
                           rect.top  + rect.height/2.f);

        auto texRect = sprite.getTextureRect();
        // uniform scale: gebruik de kleinst passende factor zodat sprite niet vervormt
        float sx = rect.width / texRect.width;
        float sy = rect.height / texRect.height;
        float s = std::min(sx, sy);
        sprite.setScale(s, s);
        win.draw(sprite);
    }

    void PacmanView::startDeath() {
        deathPlaying = true;
        deathFrameIndex = 0;
        animTimer = 0.0;
    }

    void PacmanView::stopDeath() {
        deathPlaying = false;
        deathFrameIndex = 0;
        animTimer = 0.0;
    }

    double PacmanView::getDeathDuration() const {
        return static_cast<double>(NUM_DEATH_FRAMES) * frameTime;
    }

    void PacmanView::onNotify(int event) {
        if (event == static_cast<int>(logic::Event::PACMAN_DIED)) startDeath();
        else if (event == static_cast<int>(logic::Event::PACMAN_REVIVED)) stopDeath();
    }


    void PacmanView::updateSprite(double dt) {
        if (deathPlaying) {
            animTimer += dt;
            if (animTimer >= frameTime) {
                animTimer -= frameTime;
                deathFrameIndex++;
                if (deathFrameIndex >= NUM_DEATH_FRAMES) {
                    deathFrameIndex = NUM_DEATH_FRAMES - 1; // hold last frame until world resets model
                }
            }

            // Death frames are on the first row after the normal frames
            int deathStartX = NUM_FRAMES * FRAME_SIZE;
            int frameX = deathStartX + deathFrameIndex * FRAME_SIZE;
            int frameY = 0 * FRAME_SIZE;

            sprite.setTextureRect(sf::IntRect(frameX, frameY, FRAME_SIZE, FRAME_SIZE));
            return;
        }

        animTimer += dt;

        // Ga naar volgende frame
        if (animTimer >= frameTime) {
            animTimer = 0.0;
            frameIndex = (frameIndex + 1) % NUM_FRAMES;
        }

        // als pacman tegen een muur loopt, dan moet de huidige direction behouden worden
        if (model->getDirection() != logic::Direction::NONE) {
            lastDirection = model->getDirection();
        } else {
            model->setDirection(lastDirection);
        }

        int frameX = frames[frameIndex];  // pak de juiste kolom
        int frameY = 0;

        switch (model->getDirection()) {
            case logic::Direction::RIGHT: frameY = 0 * FRAME_SIZE; break;
            case logic::Direction::LEFT:  frameY = 1 * FRAME_SIZE; break;
            case logic::Direction::UP:    frameY = 2 * FRAME_SIZE; break;
            case logic::Direction::DOWN:  frameY = 3 * FRAME_SIZE; break;
            default: return; // geen beweging -> animatie niet updaten
        }

        sprite.setTextureRect(sf::IntRect(frameX, frameY, FRAME_SIZE, FRAME_SIZE));
    }
}