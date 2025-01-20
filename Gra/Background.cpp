#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

class Star {
public:
    sf::CircleShape shape;

    Star(float x, float y, float size) {
        shape.setRadius(size);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
    }

    void update(float deltaTime, float speed) {
        shape.move(0, speed * deltaTime);
    }

    bool isOutOfBounds(float windowHeight) const {
        return shape.getPosition().y > windowHeight;
    }
};

class Background {
private:
    std::vector<Star> stars;
    float spawnCooldown;
    float spawnRate;

public:
    Background() : spawnCooldown(0.0f), spawnRate(0.05f) {
        std::srand(static_cast<unsigned>(std::time(nullptr))); // losowoœæ
    }

    void update(float deltaTime, float windowWidth, float windowHeight) {
        // Spawn nowych stars
        spawnCooldown -= deltaTime;
        if (spawnCooldown <= 0.0f) {
            spawnCooldown = spawnRate;
            float x = static_cast<float>(std::rand() % static_cast<int>(windowWidth));
            float size = static_cast<float>(std::rand() % 3 + 1); // Random size between 1 and 3
            stars.emplace_back(x, 0.0f, size);
        }

        // Update  stars
        for (auto& star : stars) {
            star.update(deltaTime, 100.0f); // Speed of falling stars
        }

        // Remove stars 
        stars.erase(
            std::remove_if(stars.begin(), stars.end(),
                [windowHeight](const Star& star) { return star.isOutOfBounds(windowHeight); }),
            stars.end());
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& star : stars) {
            window.draw(star.shape);
        }
    }
};