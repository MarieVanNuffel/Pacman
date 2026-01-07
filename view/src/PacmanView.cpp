//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "logic/PacManModel.h"
#include "../include/view/PacmanView.h"
#include "logic/Events.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "view/Resource.h"

namespace view {
    PacmanView::PacmanView(std::shared_ptr<logic::PacManModel> m)
        : EntityView(m), model(m)
    {
        texturePtr = view::Resource::loadTexture("view/assets/pacman.png");
        if (texturePtr) sprite.setTexture(*texturePtr);

        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_SIZE, FRAME_SIZE));
        sprite.setOrigin(FRAME_SIZE / 2.f, FRAME_SIZE / 2.f);
    }

    void PacmanView::draw(sf::RenderWindow& win, const Camera& cam)
    {
        auto m = model.lock();
        if (!m) return;

        double sizeW = 0.9;
        double sizeH = 0.9;

        // tile center (zoals in logic)
        double tileCenterX = m->getX();
        double tileCenterY = m->getY();

        // converteer naar genormalizeerde coords
        auto [nx, ny] = cam.tileToNormalized(tileCenterX, tileCenterY);

        // grootte als fractie van de maze
        double wFrac = sizeW / static_cast<double>(cam.mazeWidth);
        double hFrac = sizeH / static_cast<double>(cam.mazeHeight);

        // gebruik genormalizeerde helper in de Camera
        sf::FloatRect rect = cam.normalizedToPixels(nx, ny, wFrac, hFrac);

        sprite.setPosition(rect.left + rect.width/2.f,
                           rect.top  + rect.height/2.f);

        auto texRect = sprite.getTextureRect();
        // uniforme schaling
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
        auto m = model.lock();
        if (!m) return;

        if (deathPlaying) {
            animTimer += dt;
            if (animTimer >= frameTime) {
                animTimer -= frameTime;
                deathFrameIndex++;
                if (deathFrameIndex >= NUM_DEATH_FRAMES) {
                    deathFrameIndex = NUM_DEATH_FRAMES - 1; // De laatste frame wordt vastgehouden tot de reset gebeurd
                }
            }

            int deathStartX = NUM_FRAMES * FRAME_SIZE;
            int frameX = deathStartX + deathFrameIndex * FRAME_SIZE;
            int frameY = 0 * FRAME_SIZE;

            sprite.setTextureRect(sf::IntRect(frameX, frameY, FRAME_SIZE, FRAME_SIZE));
            return;
        }

        animTimer += dt;

        // ga naar volgende frame
        if (animTimer >= frameTime) {
            animTimer = 0.0;
            frameIndex = (frameIndex + 1) % NUM_FRAMES;
        }

        // als pacman tegen een muur loopt, dan moet de huidige direction behouden worden
        if (m->getDirection() != logic::Direction::NONE) {
            lastDirection = m->getDirection();
        } else {
            m->setDirection(lastDirection);
        }

        int frameX = frames[frameIndex];  // pak de juiste kolom
        int frameY = 0;

        switch (m->getDirection()) {
            case logic::Direction::RIGHT: frameY = 0 * FRAME_SIZE; break;
            case logic::Direction::LEFT:  frameY = 1 * FRAME_SIZE; break;
            case logic::Direction::UP:    frameY = 2 * FRAME_SIZE; break;
            case logic::Direction::DOWN:  frameY = 3 * FRAME_SIZE; break;
            default: return; // geen beweging, dus animatie niet updaten
        }
        sprite.setTextureRect(sf::IntRect(frameX, frameY, FRAME_SIZE, FRAME_SIZE));
    }
}
