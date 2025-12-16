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
     // animTimer += dt;
     //
     // // Placeholder animatie (later kan je frames toevoegen)
     // if (animTimer > frameTime)
     //     animTimer = 0.0;
     //     frameIndex = (frameIndex + 1) % NUM_FRAMES;
     //
     // int rectLeft = 0;
     // int rectTop = 0;
     //
     // switch (lastDirection) {
     //     case Direction::RIGHT && model->getGhostType() == GhostModel::GhostType::LockedRandom: rectLeft = frameIndex * FRAME_SIZE; break;
     //     case Direction::LEFT:  rectLeft = 2*FRAME_SIZE + frameIndex*FRAME_SIZE; break;
     //     case Direction::UP:    rectLeft = 4*FRAME_SIZE + frameIndex*FRAME_SIZE; break;
     //     case Direction::DOWN:  rectLeft = 6*FRAME_SIZE + frameIndex*FRAME_SIZE; break;
     //     default: rectLeft = 0; break;
     // }
     //
     // sprite.setTextureRect(sf::IntRect(rectLeft, rectTop, FRAME_SIZE, FRAME_SIZE));

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
    sprite.setScale(rect.width / sprite.getLocalBounds().width,
                    rect.height / sprite.getLocalBounds().height);


    win.draw(sprite);
}



