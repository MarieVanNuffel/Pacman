//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_LEVELSTATE_H
#define PACMAN_LEVELSTATE_H


#pragma once
#include "view/State.h"
#include "logic/World.h"

namespace view {

    class StateManager;

    class LevelState : public State {
    public:
        LevelState(StateManager&, sf::RenderWindow&);

        void update() override;
        void draw(sf::RenderWindow&) override;

    private:
        logic::World world_;
    };

}



#endif //PACMAN_LEVELSTATE_H