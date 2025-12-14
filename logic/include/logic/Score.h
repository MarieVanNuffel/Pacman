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
    void coinCollected();
    void fruitCollected();
    void pacmanDied();
    void ghostEaten();


    int getScore() const { return currentScore; }
    void loadHighScores();
    void saveHighScores();

    void onNotify(int event) override {
        if(event == 1) currentScore += 10; // coin collected
        else if(event == 2) currentScore += 50; // fruit collected
        // voeg meer events toe
    }
private:
    int currentScore = 0;
    std::vector<int> highScores;
    double lastCoinTime = 0.0;
};



#endif //PACMANGAME_SCORE_H