//
// Created by Marie Van Nuffel on 30/11/2025.
//

#include "../include/view/MazeView.h"


MazeView::MazeView(const World* w)
    : world(w)
{
    // Laad texture voor muren
    if (!wallTexture.loadFromFile("view/assets/wall.png")) {
        throw std::runtime_error("Failed to load wall texture!");
    }

    wallSprite.setTexture(wallTexture);
    // Optioneel: centreer sprite, afhankelijk van hoe je worldToPixels werkt
    sf::FloatRect bounds = wallSprite.getLocalBounds();
    wallSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void MazeView::draw(sf::RenderWindow& win, const Camera& cam)
{
    const auto& maze = world->getMaze();
    int mazeHeight = maze.size();
    int mazeWidth = maze[0].size();

    for (int y = 0; y < mazeHeight; ++y) {
        for (int x = 0; x < mazeWidth; ++x) {
            if (maze[y][x] != 1) continue; // Alleen muren

            double wx = (double)x / mazeWidth * 2.0 - 1.0 + 1.0 / mazeWidth;
            double wy = (double)y / mazeHeight * 2.0 - 1.0 + 1.0 / mazeHeight;
            double w = 2.0 / mazeWidth;
            double h = 2.0 / mazeHeight;

            sf::FloatRect rect = cam.worldToPixels(wx, wy, w, h);

            sf::Sprite sprite(wallTexture);  // **maak nieuwe sprite**
            sprite.setOrigin(wallTexture.getSize().x / 2.f, wallTexture.getSize().y / 2.f);
            sprite.setPosition(rect.left + rect.width / 2.f,
                               rect.top  + rect.height / 2.f);
            sprite.setScale(rect.width / wallTexture.getSize().x,
                            rect.height / wallTexture.getSize().y);

            win.draw(sprite);
        }
    }
}
