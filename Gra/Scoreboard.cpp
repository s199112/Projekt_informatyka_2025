#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>

extern const int WINDOW_WIDTH;


class Scoreboard {
public:
    static Scoreboard& getInstance() {
        static Scoreboard instance("scores.txt");
        return instance;
    }

    void addScore(const std::string& name, int score) {
        // Dodanie wyniku do listy
        scores.emplace_back(name, score);

        // Posortowanie wyników (malej¹co po punkcie)
        std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
            return b.second < a.second; // Sortowanie po wynikach malej¹co
            });

        // Ograniczenie do 50 najlepszych wyników
        if (scores.size() > 50) {
            scores.pop_back(); // Trzymaj tylko top 50 wyników
        }

        save();
    }

    void draw(sf::RenderWindow& window, const sf::Font& font, const std::string& playerName, int playerScore) {
        // Wyœwietlanie top 3 wyników
        sf::Text title("Top 3 Scores", font, 35);
        title.setFillColor(sf::Color::White);
        title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 50);
        window.draw(title);

        for (size_t i = 0; i < std::min(scores.size(), size_t(3)); ++i) {
            sf::Text scoreText(std::to_string(i + 1) + ". " + scores[i].first + ": " + std::to_string(scores[i].second),
                font, 20);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(WINDOW_WIDTH / 2 - scoreText.getGlobalBounds().width / 2, 100 + i * 30);
            window.draw(scoreText);
        }

        // Wyœwietlanie wyniku gracza
        sf::Text playerText("Your Score: " + std::to_string(playerScore), font, 30);
        playerText.setFillColor(sf::Color::Cyan);
        playerText.setPosition(WINDOW_WIDTH / 2 - playerText.getGlobalBounds().width / 2, 200);
        window.draw(playerText);

        // Wyœwietlanie miejsca gracza

        int rank = getPlayerRank(playerName, playerScore);
        if (rank != -1) {
            sf::Text rankText("Your Rank: #" + std::to_string(rank), font, 25);
            rankText.setFillColor(sf::Color::Yellow);
            rankText.setPosition(WINDOW_WIDTH / 2 - rankText.getGlobalBounds().width / 2, 240);
            window.draw(rankText);
        }
    }

    // £adowanie wyników z pliku
    void load() {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Could not load scores file.\n";
            return;
        }

        std::string name;
        int score;
        while (inFile >> name >> score) {
            scores.emplace_back(name, score); // Wczytywanie nazw i punktów
        }
    }

private:
    std::string filename;
    std::vector<std::pair<std::string, int>> scores;

    Scoreboard(const std::string& file) : filename(file) {
        load(); // £adowanie wyników z pliku przy starcie gry
    }

    void save() const {
        std::ofstream outFile(filename, std::ios::trunc); // Trunc - nadpisanie pliku
        if (!outFile) {
            std::cerr << "Could not save scores file.\n";
            return;
        }

        // Zapisanie wyników do pliku
        for (const auto& pair : scores) {
            outFile << pair.first << " " << pair.second << "\n";
        }
    }

    // Usuwamy mo¿liwoœæ kopiowania i przypisania
    Scoreboard(const Scoreboard&) = delete;
    Scoreboard& operator=(const Scoreboard&) = delete;

    int getPlayerRank(const std::string& playerName, int playerScore) {
        for (size_t i = 0; i < scores.size(); ++i) {
            if (scores[i].first == playerName && scores[i].second == playerScore) {
                return i + 1; // Ranking gracza (1-based index)
            }
        }

        return -1; // Jeœli gracz nie znajduje siê w tabeli
    }
};