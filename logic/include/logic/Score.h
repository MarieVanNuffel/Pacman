//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_SCORE_H
#define PACMANGAME_SCORE_H


#pragma once


#include <vector>
#include <string>

#include "Observer.h"


class Score : public Observer {
public:
    Score();

    // score mutators
    void coinCollected(double dt);
    void fruitCollected();
    void ghostEaten();
    void pacmanDied();

    // Observer interface
    void onNotify(int event) override;

    // highscore persistence (simple in-memory API)
    void loadHighScores();
    void saveHighScores();

    // getters
    int getCurrentScore() const { return currentScore; }
    const std::vector<int>& getHighScores() const { return highScores; }

private:
    int currentScore = 0;
    std::vector<int> highScores;
};




#endif //PACMANGAME_SCORE_H