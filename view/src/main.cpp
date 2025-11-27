//
// Created by Marie Van Nuffel on 27/11/2025.
//

//#include "view/Game.h"

//int main() {
    //view::Game game;
    //game.run();
//}

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow win(sf::VideoMode(600, 600), "Test");
    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e))
            if (e.type == sf::Event::Closed)
                win.close();

        win.clear();
        win.display();
    }
}
