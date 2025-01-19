#include <fstream>
#include <string>



class GameState {
private:
    std::string playerName;
    int score;
    int lives;
    int level;
    int enemyCount;

public:
    // Konstruktor
    GameState(const std::string& playerName = "", int score = 0, int lives = 0, int level = 0)
        : playerName(playerName), score(score), lives(lives), level(level) {}

    // Ustawienie stanu gry
    void setState(const std::string& playerName, int score, int lives, int level) {
        this->playerName = playerName;
        this->score = score;
        this->lives = lives;
        this->level = level;

    }

    // Zapis do pliku
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::out);
        if (file.is_open()) {
            file << playerName << "\n";
            file << score << "\n";
            file << lives << "\n";
            file << level << "\n";

            file.close();
        }
    }

    // Wczytanie z pliku
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::in);
        if (file.is_open()) {
            std::getline(file, playerName);
            file >> score >> lives >> level;
            file.close();
            return true;
        }
        return false;
    }

    // Getter-y
    std::string getPlayerName() const { return playerName; }
    int getScore() const { return score; }
    int getLives() const { return lives; }
    int getLevel() const { return level; }

};