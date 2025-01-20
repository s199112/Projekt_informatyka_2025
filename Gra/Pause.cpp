#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class PauseMenu {
private:
    sf::Font font;
    sf::Text pauseText;
    sf::Text continueText;
    sf::Text quitText;
    sf::Text saveText;
    sf::Text loadText;

    sf::RectangleShape background;

public:
    PauseMenu(sf::RenderWindow& window) {
        if (!font.loadFromFile("Retro Gaming.ttf")) {
            std::cerr << "Failed to load font.\n";
        }

        pauseText.setFont(font);
        pauseText.setString("Paused");
        pauseText.setCharacterSize(50);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setPosition(window.getSize().x / 2 - pauseText.getGlobalBounds().width / 2, 100);

        continueText.setFont(font);
        continueText.setString("Press [C] to Continue");
        continueText.setCharacterSize(30);
        continueText.setFillColor(sf::Color::Green);
        continueText.setPosition(window.getSize().x / 2 - continueText.getGlobalBounds().width / 2, 200);

        quitText.setFont(font);
        quitText.setString("Press [Q] to Quit");
        quitText.setCharacterSize(30);
        quitText.setFillColor(sf::Color::Red);
        quitText.setPosition(window.getSize().x / 2 - quitText.getGlobalBounds().width / 2, 250);

        saveText.setFont(font);
        saveText.setString("Press [S] to Save");
        saveText.setCharacterSize(30);
        saveText.setFillColor(sf::Color::Cyan);
        saveText.setPosition(window.getSize().x / 2 - saveText.getGlobalBounds().width / 2, 300);

        loadText.setFont(font);
        loadText.setString("Press [L] to LOAD");
        loadText.setCharacterSize(30);
        loadText.setFillColor(sf::Color::Blue);
        loadText.setPosition(window.getSize().x / 2 - loadText.getGlobalBounds().width / 2, 350);

        background.setSize(sf::Vector2f(window.getSize()));
        background.setFillColor(sf::Color(0, 0, 0, 150)); // Pó³przezroczyste t³o
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(pauseText);
        window.draw(continueText);
        window.draw(quitText);
        window.draw(saveText);
        window.draw(loadText);
    }
};
class Exit {
private:
    sf::Font font;
    sf::Text exitText;
    sf::Text EcontinueText;
    sf::Text ExitText;


    sf::RectangleShape background;

public:
    Exit(sf::RenderWindow& window) {
        if (!font.loadFromFile("Retro Gaming.ttf")) {
            std::cerr << "Failed to load font.\n";
        }

        exitText.setFont(font);
        exitText.setString("Do you want to Exit?");
        exitText.setCharacterSize(50);
        exitText.setFillColor(sf::Color::White);
        exitText.setPosition(window.getSize().x / 2 - exitText.getGlobalBounds().width / 2, 100);

        EcontinueText.setFont(font);
        EcontinueText.setString("NOO:   Press [C] to Continue");
        EcontinueText.setCharacterSize(30);
        EcontinueText.setFillColor(sf::Color::Green);
        EcontinueText.setPosition(window.getSize().x / 2 - EcontinueText.getGlobalBounds().width / 2, 200);

        ExitText.setFont(font);
        ExitText.setString("YES:   Press [Esc] to Exit");
        ExitText.setCharacterSize(30);
        ExitText.setFillColor(sf::Color::Red);
        ExitText.setPosition(window.getSize().x / 2 - ExitText.getGlobalBounds().width / 2, 250);



        background.setSize(sf::Vector2f(window.getSize()));
        background.setFillColor(sf::Color(0, 0, 0)); // Pó³przezroczyste t³o
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(exitText);
        window.draw(EcontinueText);
        window.draw(ExitText);


    }

};
