#include "view/GhostDoorView.h"
#include "logic/GhostDoorModel.h"
#include "view/Camera.h"
#include <SFML/Graphics.hpp>

namespace view {
    GhostDoorView::GhostDoorView(logic::GhostDoorModel* m)
        : EntityView(m), model(m)
    {}

    void GhostDoorView::draw(sf::RenderWindow& win, const Camera& cam) {
        if (!model) return;

        // Deur = dun balkje in tile
        sf::FloatRect rect = cam.worldToPixels(
            model->getX() - 0.5,
            model->getY() - 0.5,
            1.0,
            0.2
        );

        sf::RectangleShape bar;
        bar.setPosition(rect.left, rect.top);
        bar.setSize({rect.width, rect.height});
        bar.setFillColor(sf::Color(255, 180, 255)); // roze / paars

        win.draw(bar);
    }

    void GhostDoorView::updateSprite(double dt) {
    }
}