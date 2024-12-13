#include "game.hpp"

Game::Game(const std::string& word) : secretWord(word) {}

std::pair<int, int> Game::countBullsAndCows(const std::string& guess) const {
    int bulls = 0, cows = 0;
    for (size_t i = 0; i < guess.size(); ++i) {
        if (guess[i] == secretWord[i]) {
            ++bulls;
        } else if (secretWord.find(guess[i]) != std::string::npos) {
            ++cows;
        }
    }
    return {bulls, cows};
}

bool Game::isWordGuessed(const std::string& guess) const {
    return guess == secretWord;
}

std::string Game::getWord() const {
    return secretWord;
}
