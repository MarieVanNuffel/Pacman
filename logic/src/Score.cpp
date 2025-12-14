//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/Score.h"
#include <fstream>


Score::Score() {
    loadHighScores();
}


void Score::coinCollected() {
    currentScore += 10;
}
void Score::fruitCollected() { currentScore += 100; }
void Score::pacmanDied() { currentScore -= 50; }
void Score::ghostEaten() { currentScore += 200; }


void Score::loadHighScores() {
    // minimal implementation - try to open file
    highScores = {0,0,0,0,0};
}
void Score::saveHighScores() {
    // write to file
}