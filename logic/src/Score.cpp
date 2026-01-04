#include "logic/Score.h"
#include <algorithm>
#include <fstream>

namespace logic {
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
        currentScore -= 100;
    }

    void Score::levelCleared() {
        currentScore += 1000;
    }

    void Score::onNotify(int event) {
        // events zonder timing
        if (event == 2) fruitCollected();
        else if (event == 3) ghostEaten();
    }


    void Score::loadHighScores() {
        highScores.clear();
        std::ifstream file("highscores.txt"); // schrijf highscores naar een file

        int score;
        while (file >> score) {
            highScores.push_back(score);
        }

        while (highScores.size() < 5) // hoogste 5 worden getoont
            highScores.push_back(0);
    }

    void Score::saveHighScores() {
        highScores.push_back(currentScore);
        std::sort(highScores.rbegin(), highScores.rend()); // hoog naar laag

        if (highScores.size() > 5) // hoogste 5
            highScores.resize(5);

        std::ofstream file("highscores.txt", std::ios::trunc);
        for (int s : highScores)
            file << s << "\n";

        currentScore = 0; // reset voor nieuw spel
    }
}

