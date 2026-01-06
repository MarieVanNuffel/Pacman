#include "view/GhostDoorView.h"
#include "logic/GhostDoorModel.h"
#include "view/Camera.h"
#include <SFML/Graphics.hpp>

namespace view {
    GhostDoorView::GhostDoorView(std::shared_ptr<logic::GhostDoorModel> m)
        : EntityView(m), model(m)
    {}

    void GhostDoorView::draw(sf::RenderWindow& win, const Camera& cam) {
        auto m = model.lock();
        if (!m) return;

        // Ghostdeur is een dun balkje dat tussen de bovenkant van de muren zit
        sf::FloatRect rect = cam.worldToPixels(
            m->getX() - 0.5,
            m->getY() - 0.5,
            1.0,
            0.2
        );

        sf::RectangleShape bar;
        bar.setPosition(rect.left, rect.top);
        bar.setSize({rect.width, rect.height});
        bar.setFillColor(sf::Color(255, 180, 255)); // roos / paars

        win.draw(bar); // tekent de ghostdeur
    }

    void GhostDoorView::updateSprite(double dt) {
        // statisch, dus doet niets
    }
}