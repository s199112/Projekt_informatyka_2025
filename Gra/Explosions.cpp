#include <SFML/Graphics.hpp>

extern const float ENEMY_SIZE;
class Explosion {
public:
    sf::CircleShape shape;
    float lifetime;

    Explosion(float x, float y) : lifetime(0.15f) { // Eksplozja trwa 0.2 sekundy
        shape.setRadius(ENEMY_SIZE + 5);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin(ENEMY_SIZE - 10, ENEMY_SIZE - 10);
        shape.setPosition(x, y);
    }

    void update(float deltaTime) {
        lifetime -= deltaTime;
    }

    bool isAlive() const {
        return lifetime > 0;
    }
};