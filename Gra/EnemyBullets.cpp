#include <SFML/Graphics.hpp>
extern const int WINDOW_HEIGHT;

class EnemyBullet{
public:
    sf::Sprite sprite;
    bool active;


    EnemyBullet(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.018f, 0.018f);
        sprite.setPosition(x, y);
        active = true;
    }

    void update(float deltaTime) {
        sprite.move(0, 140.0f * deltaTime); // ruch w dó³
        if (sprite.getPosition().y > WINDOW_HEIGHT) {
            active = false; // Dezaktywacja, jeœli wyjdzie poza ekran
        }
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
};