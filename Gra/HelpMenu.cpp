#include <SFML/Graphics.hpp>
#include <iostream>

class HelpMenu {
private:
    sf::Font font;
    sf::Text helpText;
    sf::Text exitText;
    sf::Text moveText;
    sf::Text shootText;
    sf::Text spaceText;

    sf::RectangleShape background;
    sf::ConvexShape arrowUp;
    sf::ConvexShape arrowLeft;
    sf::ConvexShape arrowRight;
    sf::ConvexShape arrowDown;
    sf::ConvexShape spaceBar;

public:
    HelpMenu(sf::RenderWindow& window) {
        if (!font.loadFromFile("Retro Gaming.ttf")) {
            std::cerr << "Failed to load font.\n";
        }


        helpText.setFont(font);
        helpText.setString("HELP");
        helpText.setCharacterSize(50);
        helpText.setFillColor(sf::Color::White);
        helpText.setPosition(window.getSize().x / 2 - helpText.getGlobalBounds().width / 2, 100);

        moveText.setFont(font);
        moveText.setString(" - Poruszanie sie gracza");
        moveText.setCharacterSize(20);
        moveText.setFillColor(sf::Color::White);
        moveText.setPosition(window.getSize().x / 2 - moveText.getGlobalBounds().width / 2, 200);

        shootText.setFont(font);
        shootText.setString(" - Strzelanie");
        shootText.setCharacterSize(20);
        shootText.setFillColor(sf::Color::White);
        shootText.setPosition(window.getSize().x / 2 - shootText.getGlobalBounds().width / 2, 250);

        spaceText.setFont(font);
        spaceText.setString(" Spacebar");
        spaceText.setCharacterSize(20);
        spaceText.setFillColor(sf::Color::Black);
        spaceText.setPosition(window.getSize().x / 2 - 350, 260);

        exitText.setFont(font);
        exitText.setString("Press [C] to Continue");
        exitText.setCharacterSize(30);
        exitText.setFillColor(sf::Color::Green);
        exitText.setPosition(window.getSize().x / 2 - exitText.getGlobalBounds().width / 2, 350);


        background.setSize(sf::Vector2f(window.getSize()));
        background.setFillColor(sf::Color(0, 0, 0)); 


        arrowUp.setPointCount(3);
        arrowUp.setPoint(0, sf::Vector2f(30, 10));
        arrowUp.setPoint(1, sf::Vector2f(10, 30));
        arrowUp.setPoint(2, sf::Vector2f(50, 30));
        arrowUp.setFillColor(sf::Color::White);
        arrowUp.setPosition(window.getSize().x / 2 - moveText.getGlobalBounds().width, 188);


        arrowLeft.setPointCount(3);
        arrowLeft.setPoint(0, sf::Vector2f(30, 10));
        arrowLeft.setPoint(1, sf::Vector2f(10, 30));
        arrowLeft.setPoint(2, sf::Vector2f(30, 50));
        arrowLeft.setFillColor(sf::Color::White);
        arrowLeft.setPosition(window.getSize().x / 2 - moveText.getGlobalBounds().width - 25, 190);

        arrowRight.setPointCount(3);
        arrowRight.setPoint(0, sf::Vector2f(30, 10));
        arrowRight.setPoint(1, sf::Vector2f(50, 30));
        arrowRight.setPoint(2, sf::Vector2f(30, 50));
        arrowRight.setFillColor(sf::Color::White);
        arrowRight.setPosition(window.getSize().x / 2 - moveText.getGlobalBounds().width + 25, 190);

        arrowDown.setPointCount(3);
        arrowDown.setPoint(0, sf::Vector2f(30, 30));
        arrowDown.setPoint(1, sf::Vector2f(10, 10));
        arrowDown.setPoint(2, sf::Vector2f(50, 10));
        arrowDown.setFillColor(sf::Color::White);
        arrowDown.setPosition(window.getSize().x / 2 - moveText.getGlobalBounds().width, 210);

        spaceBar.setPointCount(4);
        spaceBar.setPoint(0, sf::Vector2f(10, 10));
        spaceBar.setPoint(1, sf::Vector2f(10, 50));
        spaceBar.setPoint(2, sf::Vector2f(210, 50));
        spaceBar.setPoint(3, sf::Vector2f(210, 10));
        spaceBar.setFillColor(sf::Color::White);
        spaceBar.setPosition(window.getSize().x / 2 - 400 , 250);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(helpText);
        window.draw(moveText);
        window.draw(exitText);
        window.draw(arrowUp);
        window.draw(arrowLeft);
        window.draw(arrowRight);
        window.draw(arrowDown);
        window.draw(shootText);
        window.draw(spaceBar);
        window.draw(spaceText);
        

    }

};