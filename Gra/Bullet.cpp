#include <SFML/Graphics.hpp>

extern const float BULLET_SPEED;
extern float shootCooldown; // Pocz�tkowy cooldown w sekundach
extern const float SHOOT_COOLDOWN_TIME; // Sta�y czas pomi�dzy strza�ami 
extern const sf::Vector2f BULLET_SIZE;
class Bullet {
public:
    sf::RectangleShape shape;
    Bullet(float x, float y) {
        shape.setSize(BULLET_SIZE);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(x, y);
    }

    void update(float deltaTime) {
        shape.move(0, -BULLET_SPEED * deltaTime);
    }
};