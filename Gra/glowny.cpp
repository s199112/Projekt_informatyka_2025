#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

// Rozmiary okna gry
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;


// Gracz
const float PLAYER_SPEED = 200.0f;
const sf::Vector2f BULLET_SIZE(5, 10);
int lives= 5;
int currentLevel = 1;


// Pocisk
const float BULLET_SPEED = 300.0f;
float shootCooldown = 0.0f; // Pocz¹tkowy cooldown w sekundach
const float SHOOT_COOLDOWN_TIME = 0.3f; // Sta³y czas pomiêdzy strza³ami 


// Wrogowie
const float ENEMY_BASE_SPEED = 40.0f;
const int ENEMY_ROWS = 5;
const int ENEMY_COLUMNS = 10;
const float ENEMY_SIZE = 15.0f;
float enemyDirection = 0.8f; // Kierunek poruszania wrogów
int score = 0;
float enemyMoveCooldown = 0.6f; // Czas miêdzy ruchami przeciwników
float enemyMoveTimer = 0.0f;

// Pickup
sf::Texture pickupTexture;

int pickupsOnLevel = 0; // Licznik Pickupów na poziom

// Klasy do gry
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
//eksplozje
class Explosion {
public:
    sf::CircleShape shape;
    float lifetime;

    Explosion(float x, float y) : lifetime(0.15f) { // Eksplozja trwa 0.2 sekundy
        shape.setRadius(ENEMY_SIZE+5);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin(ENEMY_SIZE-10, ENEMY_SIZE-10);
        shape.setPosition(x, y);
    }

    void update(float deltaTime) {
        lifetime -= deltaTime;
    }

    bool isAlive() const {
        return lifetime > 0;
    }
};
// Pickupy
class Pickup {
public:
    sf::Sprite sprite;
    bool active;

    Pickup(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.02f, 0.02f);
        sprite.setPosition(x, y);
        active = true;
    }

    void update(float deltaTime) {
        sprite.move(0, 100.0f * deltaTime); // Powolny ruch w dó³
        if (sprite.getPosition().y > WINDOW_HEIGHT) {
            active = false; // Dezaktywacja, jeœli wyjdzie poza ekran
        }
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
};

int main() {
    // Inicjalizacja okna gry
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Space Invaders");
    window.setFramerateLimit(60);
    //tekstura przeciwników
    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("enemy.png")) {
        std::cerr << "Failed to load enemy texture!" << std::endl;
        return -1; // Zakoñcz program, jeœli nie uda siê za³adowaæ tekstury
    }
    // Inicjalizacja gracza
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("player.png")) { 
        std::cerr << "Failed to load player texture!" << std::endl;
        return -1; // Zakoñcz program, jeœli nie uda siê za³adowaæ tekstury
    }
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(0.07f, 0.07f); // Skalowanie
    playerSprite.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 60);

    //inicjalizacja pickupów
    if (!pickupTexture.loadFromFile("pickup.png")) { 
        std::cerr << "Failed to load pickup texture!" << std::endl;
        return -1;
    }

    // Menu pocz¹tkowe
    bool inMenu = true;
    sf::Font font;
    font.loadFromFile("arial.ttf"); // Plik czcionki
    sf::Text title("Space Invaders", font, 50);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 100);
    sf::Text startText("Press Enter to Start", font, 30);
    startText.setPosition(WINDOW_WIDTH / 2 - startText.getGlobalBounds().width / 2, 300);

    // Ekran koñcowy
    bool gameEnded = false;
    sf::Text endText("", font, 30);
    endText.setPosition(WINDOW_WIDTH / 2 - endText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2);

    // Kontenery na pociski, wrogów i eksplozje
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Explosion> explosions;
    std::vector<Pickup> pickups;
    //kolor t³a
    sf::Color backgroundColor = sf::Color::Black; // Domyœlny kolor t³a
    float backgroundFlashTimer = 0.0f;           // Czas trwania czerwonego t³a

    
    float enemySpeed = ENEMY_BASE_SPEED;
    // Inicjalizacja przeciwników
    auto initializeEnemies = [&]() {
        enemies.clear();
        float startX = 100;
        float startY = 50;
        float spacingX = 70;
        float spacingY = 50;

        for (int row = 0; row < ENEMY_ROWS; ++row) {
            for (int col = 0; col < ENEMY_COLUMNS; ++col) {
                enemies.emplace_back(startX + col * spacingX, startY + row * spacingY, enemyTexture);
            }
        }
        };

    initializeEnemies();

    float enemyDirection = 0.8f; // Kierunek poruszania wrogów
    int score = 0;
    float enemyMoveCooldown = 0.6f; // Czas miêdzy ruchami przeciwników
    float enemyMoveTimer = 0.0f;

    // Zegar gry
    sf::Clock clock;

    while (window.isOpen()) {
        // Obs³uga zdarzeñ
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Menu pocz¹tkowe
        if (inMenu) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                inMenu = false;
            }

            // Renderowanie menu
            window.clear();
            window.draw(title);
            window.draw(startText);
            window.display();
            continue;
        }

        // Delta time
        float deltaTime = clock.restart().asSeconds();
        enemyMoveTimer += deltaTime;

        if (gameEnded) {
            window.clear();
            window.draw(endText);
            window.display();
            continue;
        }

        // Ruch gracza
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && playerSprite.getPosition().x > 0) {
            playerSprite.move(-PLAYER_SPEED * deltaTime, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && playerSprite.getPosition().x + 50 < WINDOW_WIDTH) {
            playerSprite.move(PLAYER_SPEED * deltaTime, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && playerSprite.getPosition().y > 0) {
            playerSprite.move(0, -PLAYER_SPEED * deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && playerSprite.getPosition().y + 50 < WINDOW_HEIGHT) {
            playerSprite.move(0, PLAYER_SPEED * deltaTime);
        }

        // Strzelanie
        if (shootCooldown <= 0.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (currentLevel < 10) {
                bullets.emplace_back(playerSprite.getPosition().x + 28 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                shootCooldown = SHOOT_COOLDOWN_TIME - currentLevel * 0.01f;
            }
            else {
                bullets.emplace_back(playerSprite.getPosition().x + 14 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                bullets.emplace_back(playerSprite.getPosition().x + 42 - BULLET_SIZE.x / 2, playerSprite.getPosition().y);
                shootCooldown = SHOOT_COOLDOWN_TIME - currentLevel * 0.005f;
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

        // Aktualizacja wrogów
        if (enemyMoveTimer >= enemyMoveCooldown) {
            enemyMoveTimer = 0.0f;
            bool changeDirection = false;

            for (auto& enemy : enemies) {
                if (enemy.alive) {
                    enemy.sprite.move(enemySpeed * enemyDirection, 0);
                    // Jeœli przeciwnik styka siê z krawêdzi¹, zmieñ kierunek ruchu
                    if (enemy.sprite.getPosition().x + ENEMY_SIZE * 5 > WINDOW_WIDTH-5 || enemy.sprite.getPosition().x - ENEMY_SIZE * 2 < 0) {
                        changeDirection = true;
                    }
                }
            }

            if (changeDirection) {
                enemyDirection *= -1.0f;
                for (auto& enemy : enemies) {
                    if (enemy.alive) {
                        enemy.sprite.move(0, 20); // Przesuniêcie w dó³
                    }
                }
            }
        }

        // Kolizje pocisków z wrogami i dodawanie punktów
        for (auto& bullet : bullets) {
            for (auto& enemy : enemies) {
                if (enemy.alive && bullet.shape.getGlobalBounds().intersects(enemy.getCollisionBounds())) {
                    enemy.alive = false;
                    score += 5*currentLevel;
                    bullet.shape.setPosition(-100, -100); // Usuniêcie pocisku z ekranu
                    explosions.emplace_back(enemy.sprite.getPosition().x + ENEMY_SIZE, enemy.sprite.getPosition().y + ENEMY_SIZE);
                    // Generowanie Pickupów z prawdopodobieñstwem, jeœli limit nie zosta³ osi¹gniêty
                    if (pickupsOnLevel < 5 && (std::rand() % 100) < 3) { // 30% szansa na wypadniêcie
                        pickups.emplace_back(enemy.sprite.getPosition().x+15, enemy.sprite.getPosition().y, pickupTexture);
                        pickupsOnLevel++;
                    }
                }
            }
        }

        //aktualizacja eksplozji
        for (auto it = explosions.begin(); it != explosions.end();) {
            it->update(deltaTime);
            if (!it->isAlive()) {
                it = explosions.erase(it); // Usuñ eksplozjê, gdy skoñczy siê czas
            }
            else {
                ++it;
            }
        } 
        //odejmowanie ¿yæ
       
        for (auto& enemy : enemies) {
            if (enemy.alive && enemy.getCollisionBounds().intersects(playerSprite.getGlobalBounds())) {
                enemy.alive = false;
                score += 10;
                lives-=1;
                backgroundColor = sf::Color::Red;    // Zmieñ t³o na czerwone
                playerSprite.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 60);
                backgroundFlashTimer = 0.3f;
                
            }
        }

        if (backgroundFlashTimer > 0.0f) {
            backgroundFlashTimer -= deltaTime;
            if (backgroundFlashTimer <= 0.0f) {
                backgroundColor = sf::Color::Black; // Przywróæ domyœlny kolor t³a
            }
        }
        // Aktualizacja i kolizje Pickupów
        for (auto it = pickups.begin(); it != pickups.end();) {
            it->update(deltaTime);

            if (it->active && it->getBounds().intersects(playerSprite.getGlobalBounds())) {
                // Efekt po podniesieniu Pickupu
                score += 1000 * currentLevel; // Nagroda punktowa (lub inny efekt)
                it = pickups.erase(it); // Usuñ Pickup po kolizji
            }
            else if (!it->active) {
                it = pickups.erase(it); // Usuñ nieaktywny Pickup
            }
            else {
                ++it;
            }
        }
        // Sprawdzenie warunków wygranej/przegranej
        bool allEnemiesDefeated = true;
        for (const auto& enemy : enemies) { 
            if (enemy.alive) {
                allEnemiesDefeated = false;
                if (enemy.sprite.getPosition().y + ENEMY_SIZE * 2 >= WINDOW_HEIGHT||lives<=0) {
                    gameEnded = true;
                    endText.setString("Game Over! \nFinal Score: " + std::to_string(score));
                    endText.setPosition(WINDOW_WIDTH / 2 - endText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2);
                }
            }
        } 
       
        
        if (allEnemiesDefeated) {
            currentLevel++;
            pickups.clear(); // Usuñ wszystkie pozosta³e Pickupy
            pickupsOnLevel = 0; // Zresetuj licznik
            playerSprite.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 60);
            if (enemyMoveCooldown > 0.1) {
                enemyMoveCooldown -= 0.05f; // Zwiêkszenie szybkoœci ruchu
            }
            else {
                enemyMoveCooldown -= 0.01f;
            }
            initializeEnemies();
        }

        // Renderowanie
        window.clear(backgroundColor);

        // Rysowanie gracza
        window.draw(playerSprite);

        // Rysowanie pocisków
        for (const auto& bullet : bullets) {
            window.draw(bullet.shape);
        }

        // Rysowanie wrogów
        for (const auto& enemy : enemies) {
            if (enemy.alive) {
                window.draw(enemy.sprite);
            }
        }
        // Rysowanie eksplozji
        for (const auto& explosion : explosions) {
            window.draw(explosion.shape);
        }
        // Rysowanie Pickupów
        for (const auto& pickup : pickups) {
            window.draw(pickup.sprite);
        }

        // Wyœwietlanie punktów, przeciwników, LVL i ¿yæ
        sf::Text scoreText("SCORE: " + std::to_string(score), font, 20);
        scoreText.setPosition(10, 10);
        window.draw(scoreText);
        sf::Text debugText("", font, 15);
        debugText.setPosition(10, 40);
        debugText.setString("Enemies remaining: " + std::to_string(std::count_if(enemies.begin(), enemies.end(),[](const Enemy& e) { return e.alive; })));
            
        window.draw(debugText);
        sf::Text lvlText("LEVEL: " + std::to_string(currentLevel), font, 20);
        lvlText.setPosition(WINDOW_WIDTH-120, 10);
        window.draw(lvlText);
        sf::Text livesText("LIVES: " + std::to_string(lives), font, 20);
        livesText.setPosition(WINDOW_WIDTH/2, 10);
        window.draw(livesText);

        window.display();
    }

    return 0;
}
