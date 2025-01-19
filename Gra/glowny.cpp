#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Scoreboard.cpp"
#include "Enemy.cpp"
#include "Pickups.cpp"
#include "Explosions.cpp"
#include "Pause.cpp"
#include "GameState.cpp"
#include "EnemyBullets.cpp"
#include "Player.cpp"


// Rozmiary okna gry
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;


// Gracz
const float PLAYER_SPEED = 200.0f;
int lives= 5;
int currentLevel = 1;

std::string playerName = "";
bool isEnteringName = false; // Czy gracz wprowadza imi�


// Pocisk
const float BULLET_SPEED = 300.0f;
float shootCooldown = 0.0f; // Pocz�tkowy cooldown w sekundach
const float SHOOT_COOLDOWN_TIME = 0.3f; // Sta�y czas pomi�dzy strza�ami 



// Wrogowie
const float ENEMY_BASE_SPEED = 40.0f;
const int ENEMY_ROWS = 5;
const int ENEMY_COLUMNS = 10;
const float ENEMY_SIZE = 15.0f;
float enemyDirection = 0.8f; // Kierunek poruszania wrog�w
int score = 0;
float enemyMoveCooldown = 0.6f; // Czas mi�dzy ruchami przeciwnik�w
float enemyMoveTimer = 0.0f;

// Pickup
sf::Texture pickupTexture;
int pickupsOnLevel = 0; // Licznik Pickup�w na poziom
bool pickupEffect1Active = false; // Czy efekt1 jest aktywny
bool pickupEffect2Active = false; // Czy efekt2 jest aktywny
bool pickupEffect3Active = false; // Czy efekt3 jest aktywny
float pickupEffectTimer = 0.0f; // Pozosta�y czas efektu

// Klasy do gry

//Player

 
//Pocisk

// Przeciwnik

// Eksplozje

// Pickupy

// Scoreboard

// Pauza

//zapis i �adowanie

//strza�y przeciwnik�w


int main() {
    // Inicjalizacja okna gry
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Space Invaders");
    window.setFramerateLimit(60);
    
    //tekstura przeciwnik�w
    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("enemy.png")) {
        std::cerr << "Failed to load enemy texture!" << std::endl;
        return -1; // Zako�cz program, je�li nie uda si� za�adowa� tekstury
    }
    // Inicjalizacja gracza
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("player.png")) { 
        std::cerr << "Failed to load player texture!" << std::endl;
        return -1; // Zako�cz program, je�li nie uda si� za�adowa� tekstury
    }
    Player player(playerTexture);
    

    //inicjalizacja pickup�w
    if (!pickupTexture.loadFromFile("pickup.png")) { 
        std::cerr << "Failed to load pickup texture!" << std::endl;
        return -1;
    }
    //inicjacja pocisk�w przeciwnika
    sf::Texture EB;
    if (!EB.loadFromFile("EB.png")) {
        std::cerr << "Failed to load EnemyBullet texture!" << std::endl;
        return -1;
    }
    // inicjacja zapisu gry
    GameState gameState;

    // Menu pocz�tkowe
    bool inMenu = true;
    sf::Font font;
    font.loadFromFile("arial.ttf"); // Plik czcionki
    

    // Flaga kontroluj�ca stan pauzy
    bool isPaused = false;
    bool isRunning = true;
    PauseMenu pauseMenu(window);

    // Ekran ko�cowy
    bool gameEnded = false;
    sf::Text endText("", font, 50);
    bool scoreSaved = false;
    bool gameLoaded = false;

    // Kontenery 
    
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Explosion> explosions;
    std::vector<Pickup> pickups;
    std::vector<EnemyBullet> EnemyBullets;
    //kolor t�a
    sf::Color backgroundColor = sf::Color::Black; // Domy�lny kolor t�a
    float backgroundFlashTimer = 0.0f;           // Czas trwania czerwonego t�a

    
    float enemySpeed = ENEMY_BASE_SPEED;
    // Inicjalizacja przeciwnik�w
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

    float enemyDirection = 0.8f; // Kierunek poruszania wrog�w
    int score = 0;
    float enemyMoveCooldown = 0.6f; // Czas mi�dzy ruchami przeciwnik�w
    float enemyMoveTimer = 0.0f;

    // Zegar gry
    sf::Clock clock;
    
    while (window.isOpen()) {
        // Obs�uga zdarze�
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (isEnteringName) {
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b') { // Obs�uga backspace
                        if (!playerName.empty()) {
                            playerName.pop_back();
                        }
                    }

                    else if (event.text.unicode == '\r') { // Obs�uga Enter
                        
                        isEnteringName = false;
                        inMenu = false;// Zako�czenie wprowadzania imienia
                    }
                    else if (playerName.size() < 15 && std::isprint(event.text.unicode)) {
                        playerName += static_cast<char>(event.text.unicode); // Dodaj znak
                    }
                }
            }
        }

        // Menu pocz�tkowe
        if (inMenu) {
            if (!isEnteringName && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
               isEnteringName = true;
            }
          
            // Renderowanie menu
            sf::Text title("Kosmiczni Bandyci", font, 50);
            title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 100);
            sf::Text startText("Press Enter to Start", font, 30);
            startText.setPosition(WINDOW_WIDTH / 2 - startText.getGlobalBounds().width / 2, 300);
            sf::Text namePrompt("Enter your name:", font, 30);
            namePrompt.setPosition(WINDOW_WIDTH / 2 - namePrompt.getGlobalBounds().width / 2, 200);
            sf::Text playerNameText(playerName, font, 30);
            playerNameText.setPosition(WINDOW_WIDTH / 2 - playerNameText.getGlobalBounds().width / 2, 250);
            sf::Text loadText;
            loadText.setFont(font);
            loadText.setString("Press [L] to LOAD");
            loadText.setCharacterSize(30);
            loadText.setFillColor(sf::Color::Blue);
            loadText.setPosition(window.getSize().x / 2 - loadText.getGlobalBounds().width / 2, 400);

            if (!isEnteringName) {
                window.clear();
                window.draw(title);
                window.draw(startText);
                window.draw(loadText);
            }
            else if (isEnteringName) {
                window.clear();
                window.draw(namePrompt);
                window.draw(playerNameText);
            }
            window.display();
            continue;
        }

        
            if (!isPaused && sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                isPaused = true; // Aktywacja pauzy
            }
            else if (isPaused) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                    isPaused = false; // Kontynuacja gry
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                    isRunning = false; // Wyj�cie z gry
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                gameState.setState(playerName, score, lives, currentLevel);
                gameState.saveToFile("savegame.txt");
                bool gameLoaded = false;
                backgroundColor = sf::Color::Cyan;    // Zmie� t�o na czerwone
                backgroundFlashTimer = 0.3f;//czas trwania zmiany t�a
            }
            if (inMenu||isPaused&& sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                if (gameState.loadFromFile("savegame.txt")) {
                    playerName = gameState.getPlayerName();
                    score = gameState.getScore();
                    lives = gameState.getLives();
                    currentLevel = gameState.getLevel();
                    
                }
                bool gameLoaded = true;
                backgroundColor = sf::Color::Blue;    // Zmie� t�o na czerwone
                backgroundFlashTimer = 0.3f;
                for (auto& enemy : enemies) {
                    enemy.alive = false; //czas trwania zmiany t�a
                }
                initializeEnemies();
            }

        // Delta time
        float deltaTime = clock.restart().asSeconds();
        enemyMoveTimer += deltaTime;

        //czas zmainy ekranu
        if (backgroundFlashTimer > 0.0f) {
            backgroundFlashTimer -= deltaTime;
            if (backgroundFlashTimer <= 0.0f) {
                backgroundColor = sf::Color::Black; // Przywr�� domy�lny kolor t�a
            }
        }

        if (!isPaused&&!gameEnded) {
            // Aktualizacja efektu z pickupu
            if (pickupEffect1Active || pickupEffect2Active || pickupEffect3Active) {
                pickupEffectTimer -= deltaTime;
                if (pickupEffectTimer <= 0.0f) {
                    pickupEffect1Active = false;
                    pickupEffect2Active = false;
                    pickupEffect3Active = false;// Wy��cz efekt po 10 sekundach
                }
            }

            // Ruch gracza
            float currentPlayerSpeed = PLAYER_SPEED;
            if (pickupEffect2Active) {
                currentPlayerSpeed *= 1.8f;//zwiekszenie predko�ci
            }


            player.PlayerMove(currentPlayerSpeed, deltaTime);

            // Strzelanie
            
            player.shooting(shootCooldown, deltaTime, bullets, pickupEffect1Active, pickupEffect3Active, currentLevel);
            

            // Aktualizacja wrog�w
            if (enemyMoveTimer >= enemyMoveCooldown) {
                enemyMoveTimer = 0.0f;
                bool changeDirection = false;

                for (auto& enemy : enemies) {
                    if (enemy.alive) {
                        enemy.sprite.move(enemySpeed * enemyDirection, 0);
                        // Je�li przeciwnik styka si� z kraw�dzi�, zmie� kierunek ruchu
                        if (enemy.sprite.getPosition().x + ENEMY_SIZE * 5 > WINDOW_WIDTH - 5 || enemy.sprite.getPosition().x - ENEMY_SIZE * 2 < 0) {
                            changeDirection = true;
                        }
                        if (currentLevel>=3 &&(std::rand() % 100) < 0.05 * currentLevel) { // 3% szansa na wypadni�cie
                            EnemyBullets.emplace_back(enemy.sprite.getPosition().x + 15, enemy.sprite.getPosition().y, EB);

                        }
                    }
                    
                }

                if (changeDirection) {
                    enemyDirection *= -1.0f;
                    for (auto& enemy : enemies) {
                        if (enemy.alive) {
                            enemy.sprite.move(0, 20); // Przesuni�cie w d�
                        }
                    }
                }
            }

            // Kolizje pocisk�w z wrogami i dodawanie punkt�w
            for (auto& bullet : bullets) {
                for (auto& enemy : enemies) {
                    if (enemy.alive && bullet.shape.getGlobalBounds().intersects(enemy.getCollisionBounds())) {
                        enemy.alive = false;
                        score += 5 * currentLevel;
                        bullet.shape.setPosition(-100, -100); // Usuni�cie pocisku z ekranu
                        explosions.emplace_back(enemy.sprite.getPosition().x + ENEMY_SIZE, enemy.sprite.getPosition().y + ENEMY_SIZE);
                        // Generowanie Pickup�w z prawdopodobie�stwem, je�li limit nie zosta� osi�gni�ty
                        if (pickupsOnLevel < 4 && (std::rand() % 100) < 5) { // 3% szansa na wypadni�cie
                            pickups.emplace_back(enemy.sprite.getPosition().x + 15, enemy.sprite.getPosition().y, pickupTexture);
                            pickupsOnLevel++;
                        }
                    }
                }
            }

            //aktualizacja eksplozji
            for (auto it = explosions.begin(); it != explosions.end();) {
                it->update(deltaTime);
                if (!it->isAlive()) {
                    it = explosions.erase(it); // Usu� eksplozj�, gdy sko�czy si� czas
                }
                else {
                    ++it;
                }
            }
            //odejmowanie �y�

            for (auto& enemy : enemies) {
                if (enemy.alive && enemy.getCollisionBounds().intersects(player.getBounds())) {
                    enemy.alive = false;
                    score += 10;
                    lives -= 1;

                    player.setPosition();
                    backgroundColor = sf::Color::Red;    // Zmie� t�o na czerwone
                    backgroundFlashTimer = 0.3f;//czas trwania zmiany t�a

                }
            }


            //aktualizacja i kolizje bomb
            for (auto it = EnemyBullets.begin(); it != EnemyBullets.end();) {
                it->update(deltaTime);
                if (it->active && it->getBounds().intersects(player.getBounds())) {
                    lives -= 1;
                    score -= 250 * currentLevel;
                    backgroundColor = sf::Color::Red;    // Zmie� t�o na czerwone
                    backgroundFlashTimer = 0.3f;//czas trwania zmiany t�a
                    it = EnemyBullets.erase(it);
                }
                else if (!it->active) {
                    it = EnemyBullets.erase(it); // Usu� nieaktywne
                }
                else {
                    ++it;
                }
            }
            // Aktualizacja i kolizje Pickup�w
            for (auto it = pickups.begin(); it != pickups.end();) {
                it->update(deltaTime);

                if (it->active && it->getBounds().intersects(player.getBounds())) {
                    // Efekt po podniesieniu Pickupu
                    score += 500 * currentLevel;// Nagroda
                    if (pickupsOnLevel < 4 && (std::rand() % 100) < 50) {
                        if (pickupEffect1Active && it->getBounds().intersects(player.getBounds())) {
                            pickupEffect3Active = true; //efekt3 - strzelanie 3
                            pickupEffectTimer = 10.0f;
                        }

                        pickupEffect1Active = true; //efekt1 - strzelanie 2
                        pickupEffectTimer = 10.0f;

                    }
                    else {
                        pickupEffect2Active = true; //efekt2 - szybko��
                        pickupEffectTimer = 10.0f;
                    }

                    it = pickups.erase(it); // Usu� Pickup po kolizji
                }
                else if (!it->active) {
                    it = pickups.erase(it); // Usu� nieaktywny Pickup
                }
                else {
                    ++it;
                }
            }
        }
        // Sprawdzenie warunk�w wygranej/przegranej
        bool allEnemiesDefeated = true;
        for (const auto& enemy : enemies) { 
            if (enemy.alive) {
                allEnemiesDefeated = false;
                if (enemy.sprite.getPosition().y + ENEMY_SIZE * 2 >= WINDOW_HEIGHT||lives<=0||!isRunning) {
                    gameEnded = true;
                    endText.setString("Game Over!");
                    endText.setPosition(WINDOW_WIDTH / 2 - endText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2);
                                        
                }
            }
            else if (currentLevel == 20) {
                gameEnded = true;                
                endText.setString("YOU WIN!CONGRATULATIONS!!");
                endText.setPosition(WINDOW_WIDTH / 2 - endText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2);
                                
            }
        }
        

        if (gameEnded) {
            window.clear();

            // Dodanie wyniku gracza do tablicy wynik�w, je�li jeszcze tego nie zrobili�my
            if (!scoreSaved) {
                Scoreboard::getInstance().addScore(playerName, score);
                scoreSaved = true; // Zapobiegaj zapisowi wielokrotnemu
            }

            // Wy�wietlenie tablicy wynik�w
            Scoreboard::getInstance().draw(window, font, playerName, score);
            window.draw(endText);
            window.display();
            continue;
        }
            

       
        
        if (allEnemiesDefeated) {
            currentLevel++;
            pickups.clear(); // Usu� wszystkie pozosta�e Pickupy
            EnemyBullets.clear();
            for (auto it = bullets.begin(); it != bullets.end();) {
                it = bullets.erase(it);
            }
            pickupsOnLevel = 0; // Zresetuj licznik
            player.setPosition();
            if (currentLevel <= 10) {
                if (enemyMoveCooldown > 0.1) {
                    enemyMoveCooldown -= 0.04f; // Zwi�kszenie szybko�ci ruchu
                }
            }
            initializeEnemies();
        }

        // Renderowanie
        window.clear(backgroundColor);

        if (!isPaused) {
            // Rysowanie gracza
            window.draw(player.playerSprite);

            // Rysowanie pocisk�w
            for (const auto& bullet : bullets) {
                window.draw(bullet.shape);
            }

            // Rysowanie wrog�w
            for (const auto& enemy : enemies) {
                if (enemy.alive) {
                    window.draw(enemy.sprite);
                }
            }
            // Rysowanie eksplozji
            for (const auto& explosion : explosions) {
                window.draw(explosion.shape);
            }
            // Rysowanie Pickup�w
            for (const auto& pickup : pickups) {
                window.draw(pickup.sprite);
            }
            //
            for (const auto& EB : EnemyBullets) {
                window.draw(EB.sprite);
            }

            // Wy�wietlanie punkt�w, przeciwnik�w, LVL i �y�
            sf::Text scoreText("SCORE: " + std::to_string(score), font, 20);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);
            sf::Text debugText("", font, 15);
            debugText.setPosition(10, 40);
            debugText.setString("Enemies remaining: " + std::to_string(std::count_if(enemies.begin(), enemies.end(), [](const Enemy& e) { return e.alive; })));
            window.draw(debugText);
            sf::Text lvlText("LEVEL: " + std::to_string(currentLevel), font, 20);
            lvlText.setPosition(WINDOW_WIDTH - 120, 10);
            window.draw(lvlText);
            sf::Text livesText("LIVES: " + std::to_string(lives), font, 20);
            livesText.setPosition(WINDOW_WIDTH - 120, 30);
            window.draw(livesText);
            sf::Text playerOn(playerName, font, 20);
            playerOn.setPosition(WINDOW_WIDTH / 2 - playerOn.getGlobalBounds().width / 2, 10);
            window.draw(playerOn);
            if (pickupEffect1Active) {
                sf::Text effect1Text("SHOOTING BOOST ACTIVE", font, 20);
                effect1Text.setFillColor(sf::Color::Red);
                effect1Text.setPosition(WINDOW_WIDTH / 2 - effect1Text.getGlobalBounds().width / 2, 50);
                window.draw(effect1Text);
            }
            if (pickupEffect2Active) {
                sf::Text effect2Text("SPEED BOOST ACTIVE", font, 20);
                effect2Text.setFillColor(sf::Color::Yellow);
                effect2Text.setPosition(WINDOW_WIDTH / 2 - effect2Text.getGlobalBounds().width / 2, 30);
                window.draw(effect2Text);
            }
        }
        else {
            pauseMenu.draw(window); // Renderuj menu pauzy
        }

        window.display();
    }

    return 0;
}
