//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/GhostView.h"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Texture.hpp>


GhostView::GhostView(GhostModel* m)
    : EntityView(m), model(m)
{
    if (!texture.loadFromFile("view/assets/pacman.png"))
    {
        std::cerr << "Kan ghost.png niet laden!" << std::endl;
    }

    sprite.setTexture(texture);

    int rectTop;
    switch (model->getGhostType()) {
        case GhostModel::GhostType::LockedRandom:    rectTop = 64; break;  // rood
        case GhostModel::GhostType::AheadOfPacman1:  rectTop = 80; break; // roze
        case GhostModel::GhostType::AheadOfPacman2:  rectTop = 96; break; // lichtblauw
        case GhostModel::GhostType::DirectChase:     rectTop = 112; break; // oranje
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
    if (model->getDirection() != Direction::NONE) {
        lastDirection = model->getDirection();
    }

    constexpr int FRAME = 16;
    constexpr int GHOST_BASE_Y = 64; // eerste ghostrij (rood)

    int rectLeft = 0;
    int rectTop  = 0;

    // --- KLEUR → RIJ ---
    int colorIndex = 0;
    switch (model->getGhostType()) {
        case GhostModel::GhostType::LockedRandom:   colorIndex = 0; break; // rood
        case GhostModel::GhostType::AheadOfPacman1: colorIndex = 1; break; // roze
        case GhostModel::GhostType::AheadOfPacman2: colorIndex = 2; break; // lichtblauw
        case GhostModel::GhostType::DirectChase:    colorIndex = 3; break; // oranje
    }

    rectTop = GHOST_BASE_Y + colorIndex * FRAME;

    // --- RICHTING → KOLOM ---
    switch (lastDirection) {
        case Direction::RIGHT:
            rectLeft = 0 + frameIndex * FRAME;
            break;
        case Direction::LEFT:
            rectLeft = 32 + frameIndex * FRAME;
            break;
        case Direction::UP:
            rectLeft = 64 + frameIndex * FRAME;
            break;
        case Direction::DOWN:
            rectLeft = 96 + frameIndex * FRAME;
            break;
        default:
            return;
    }

    if (model->getMode() == GhostModel::Mode::Fear) {
        rectTop = GHOST_BASE_Y;
        rectLeft = 128 + frameIndex * FRAME;
    } else if (model->getMode() == GhostModel::Mode::Eaten) {
        rectTop = 80;
        switch (lastDirection) {
            case Direction::RIGHT:
                rectLeft = 112;
                break;
            case Direction::LEFT:
                rectLeft = 128;
                break;
            case Direction::UP:
                rectLeft = 144;
                break;
            case Direction::DOWN:
                rectLeft = 160;
                break;
            default:
                return;
        }
    }

    sprite.setTextureRect(sf::IntRect(rectLeft, rectTop, FRAME, FRAME));
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



