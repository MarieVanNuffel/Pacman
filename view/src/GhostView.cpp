//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/GhostView.h"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Texture.hpp>


GhostView::GhostView(logic::GhostModel* m)
    : EntityView(m), model(m)
{
    if (!texture.loadFromFile("view/assets/pacman.png"))
    {
        std::cerr << "Kan ghost.png niet laden!" << std::endl;
    }

    sprite.setTexture(texture);

    int rectTop;
    switch (model->getGhostType()) {
        case logic::GhostModel::GhostType::LockedRandom:    rectTop = 64; break;  // rood
        case logic::GhostModel::GhostType::AheadOfPacman1:  rectTop = 80; break; // roze
        case logic::GhostModel::GhostType::AheadOfPacman2:  rectTop = 96; break; // lichtblauw
        case logic::GhostModel::GhostType::DirectChase:     rectTop = 112; break; // oranje
        default: rectTop = 0; break;
    }
    sprite.setTextureRect(sf::IntRect(0, rectTop, FRAME_SIZE, FRAME_SIZE));
    sprite.setOrigin(FRAME_SIZE / 2.f, FRAME_SIZE / 2.f);
}


void GhostView::updateSprite(double dt)
{
    animTimer += dt;

    if (animTimer >= frameTime) {
        animTimer = 0.0;
        frameIndex = (frameIndex + 1) % 2; // 2 frames: dicht/open
    }

    // richting onthouden zoals bij Pac-Man
    if (model->getDirection() != logic::Direction::NONE) {
        lastDirection = model->getDirection();
    }

    constexpr int GHOST_BASE_Y = 64; // eerste ghostrij (rood)

    int rectLeft = 0;
    int rectTop  = 0;

    if (model->getMode() == logic::GhostModel::Mode::Fear) {
        // Fear mode: blue ghost sprites
        rectTop = GHOST_BASE_Y;
        rectLeft = 128 + frameIndex * FRAME_SIZE;

        sprite.setTextureRect(sf::IntRect(rectLeft, rectTop, FRAME_SIZE, FRAME_SIZE));
        return; // ✅ Klaar, geen verdere processing
    }

    if (model->getMode() == logic::GhostModel::Mode::Eaten) {
        // Eaten mode: eyes only
        rectTop = 80;

        switch (lastDirection) {
            case logic::Direction::RIGHT:
                rectLeft = 128;
                break;
            case logic::Direction::LEFT:
                rectLeft = 144;
                break;
            case logic::Direction::UP:
                rectLeft = 160;
                break;
            case logic::Direction::DOWN:
                rectLeft = 176;
                break;
            default:
                rectLeft = 128; // default naar rechts
                break;
        }

        sprite.setTextureRect(sf::IntRect(rectLeft, rectTop, FRAME_SIZE, FRAME_SIZE));
        return; // ✅ Klaar
    }

    // --- KLEUR → RIJ ---
    int colorIndex = 0;
    switch (model->getGhostType()) {
        case logic::GhostModel::GhostType::LockedRandom:   colorIndex = 0; break; // rood
        case logic::GhostModel::GhostType::AheadOfPacman1: colorIndex = 1; break; // roze
        case logic::GhostModel::GhostType::AheadOfPacman2: colorIndex = 2; break; // lichtblauw
        case logic::GhostModel::GhostType::DirectChase:    colorIndex = 3; break; // oranje
    }

    rectTop = GHOST_BASE_Y + colorIndex * FRAME_SIZE;

    // --- RICHTING → KOLOM ---
    switch (lastDirection) {
        case logic::Direction::RIGHT:
            rectLeft = 0 + frameIndex * FRAME_SIZE;
            break;
        case logic::Direction::LEFT:
            rectLeft = 32 + frameIndex * FRAME_SIZE;
            break;
        case logic::Direction::UP:
            rectLeft = 64 + frameIndex * FRAME_SIZE;
            break;
        case logic::Direction::DOWN:
            rectLeft = 96 + frameIndex * FRAME_SIZE;
            break;
        default:
            return;
    }

    sprite.setTextureRect(sf::IntRect(rectLeft, rectTop, FRAME_SIZE, FRAME_SIZE));
}




void GhostView::draw(sf::RenderWindow& win, const Camera& cam)
{
    if (!model) return;

    double sizeW = 0.9; // 0.9 van een cel
    double sizeH = 0.9;

    sf::FloatRect rect = cam.worldToPixels(model->getX() - sizeW/2.0,
                                       model->getY() - sizeH/2.0,
                                       sizeW, sizeH);
    sprite.setPosition(rect.left + rect.width/2.f,
                       rect.top  + rect.height/2.f);

    auto texRect = sprite.getTextureRect();
    // uniform scaling to preserve aspect ratio
    float sx = rect.width / texRect.width;
    float sy = rect.height / texRect.height;
    float s = std::min(sx, sy);
    sprite.setScale(s, s);

    win.draw(sprite);
}



