#include <SFML/Graphics.hpp>
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int WINDOW_HEIGHT;
extern const float BULLET_SPEED;
extern const float SHOOT_COOLDOWN_TIME; // Sta³y czas pomiêdzy strza³ami 
const sf::Vector2f BULLET_SIZE(5, 10);
//strza³
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

//gracz
class Player {
public:
    sf::Sprite playerSprite;
    Player(const sf::Texture& texture) {

        playerSprite.setTexture(texture);
        playerSprite.setScale(0.07f, 0.07f); // Skalowanie
        playerSprite.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 60);

    }
    void setPosition() {
        playerSprite.setPosition(WINDOW_WIDTH / 2 - playerSprite.getGlobalBounds().width / 2, WINDOW_HEIGHT - 60);

    }
    void PlayerMove(float currentPlayerSpeed, float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && playerSprite.getPosition().x > 0) {
            playerSprite.move(-currentPlayerSpeed * deltaTime, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && playerSprite.getPosition().x + 50 < WINDOW_WIDTH) {
            playerSprite.move(currentPlayerSpeed * deltaTime, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && playerSprite.getPosition().y > 0) {
            playerSprite.move(0, -currentPlayerSpeed * deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && playerSprite.getPosition().y + 50 < WINDOW_HEIGHT) {
            playerSprite.move(0, currentPlayerSpeed * deltaTime);
        }
    }

    void shooting(float& shootCooldown, float deltaTime, std::vector<Bullet>& bullets, bool pickupEffect1Active, bool pickupEffect3Active, int currentLevel) {
        // Strzelanie

        if (shootCooldown <= 0.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (pickupEffect1Active && !pickupEffect3Active) {

                if (currentLevel < 10) {
                    bullets.emplace_back(playerSprite.getPosition().x + 14 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    bullets.emplace_back(playerSprite.getPosition().x + 42 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    shootCooldown = SHOOT_COOLDOWN_TIME - currentLevel * 0.007f;
                }
                else {
                    bullets.emplace_back(playerSprite.getPosition().x + 14 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    bullets.emplace_back(playerSprite.getPosition().x + 42 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    shootCooldown = SHOOT_COOLDOWN_TIME - currentLevel * 0.005f;
                }

            }
            else if (pickupEffect1Active && pickupEffect3Active) {
                if (currentLevel < 10) {
                    bullets.emplace_back(playerSprite.getPosition().x + 14 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    bullets.emplace_back(playerSprite.getPosition().x + 28 - BULLET_SIZE.x / 2, playerSprite.getPosition().y - 10);
                    bullets.emplace_back(playerSprite.getPosition().x + 42 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    shootCooldown = SHOOT_COOLDOWN_TIME - currentLevel * 0.006f;
                }
                else {
                    bullets.emplace_back(playerSprite.getPosition().x + 14 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    bullets.emplace_back(playerSprite.getPosition().x + 28 - BULLET_SIZE.x / 2, playerSprite.getPosition().y - 10);
                    bullets.emplace_back(playerSprite.getPosition().x + 42 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    shootCooldown = SHOOT_COOLDOWN_TIME - currentLevel * 0.004f;
                }
            }
            else {
                if (currentLevel < 10) {
                    bullets.emplace_back(playerSprite.getPosition().x + 28 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    shootCooldown = SHOOT_COOLDOWN_TIME - currentLevel * 0.008f;
                }
                else {
                    bullets.emplace_back(playerSprite.getPosition().x + 28 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                    shootCooldown = SHOOT_COOLDOWN_TIME - currentLevel * 0.006f;
                }
            }

        }
        // Aktualizacja cooldownu
        if (shootCooldown > 0.0f) {
            shootCooldown -= deltaTime;
        }

        // Aktualizacja pocisków
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->update(deltaTime);
            if (it->shape.getPosition().y < 0) {
                it = bullets.erase(it);
            }
            else {
                ++it;
            }
        }

    }


    sf::FloatRect getBounds() const {
        return playerSprite.getGlobalBounds();
    }

};