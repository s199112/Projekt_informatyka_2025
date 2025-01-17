#include <SFML/Graphics.hpp>
extern const float ENEMY_BASE_SPEED;
extern const int ENEMY_ROWS;
extern const int ENEMY_COLUMNS;
extern const float ENEMY_SIZE;
extern float enemyDirection; // Kierunek poruszania wrogów
extern float enemyMoveCooldown; // Czas miêdzy ruchami przeciwników
extern float enemyMoveTimer;

class Enemy {
public:
    sf::Sprite sprite;
    bool alive;

    Enemy(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.05f, 0.05f); // Skalowanie grafiki, jeœli jest zbyt du¿a
        sprite.setPosition(x, y);
        alive = true;
    }
    //hitbox
    sf::FloatRect getCollisionBounds() const {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        float padding = 5.0f; // Zmniejszenie kolizji o 5 pikseli z ka¿dej strony
        bounds.left += padding;
        bounds.top += padding;
        bounds.width -= 3 * padding;
        bounds.height -= 2 * padding;
        return bounds;
    }
};