#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include <utility>

class Game {
public:
    explicit Game(const std::string& word);
    std::pair<int, int> countBullsAndCows(const std::string& guess) const;
    bool isWordGuessed(const std::string& guess) const;
    std::string getWord() const;

private:
    std::string secretWord;
};

#endif // GAME_HPP
