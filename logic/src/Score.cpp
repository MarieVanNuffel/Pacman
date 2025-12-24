#include "logic/Score.h"
#include <algorithm>

Score::Score() {
    loadHighScores();
}

void Score::coinCollected(double timeSinceLastCoin) {
    int points = 10;

    if (timeSinceLastCoin < 0.5)      points = 50;
    else if (timeSinceLastCoin < 1.0) points = 30;
    else if (timeSinceLastCoin < 2.0) points = 20;

    currentScore += points;
}

void Score::fruitCollected() {
    currentScore += 100;
}

void Score::ghostEaten() {
    currentScore += 200;
}

void Score::pacmanDied() {
    currentScore -= 50;
}

void Score::levelCleared() {
    currentScore += 500;
}

void Score::onNotify(int event) {
    // events zonder timing
    if (event == 2) fruitCollected();
    else if (event == 3) ghostEaten();
}

void Score::loadHighScores() {
    highScores = {0,0,0,0,0};
}

void Score::saveHighScores() {
    highScores.push_back(currentScore);
    std::sort(highScores.rbegin(), highScores.rend());
    if (highScores.size() > 5)
        highScores.resize(5);
}
