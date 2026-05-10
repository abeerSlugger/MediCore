#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>

class Button {
    sf::RectangleShape box;
    sf::Text           label;
    sf::Color          normalColor;
    sf::Color          hoverColor;
public:
    Button();
    Button(float x, float y, float w, float h,
        const char* text, sf::Font& font, int fontSize = 15);
    void setPosition(float x, float y);
    void setText(const char* text);
    bool isClicked(sf::Event& e);
    void handleHover(sf::RenderWindow& win);
    void draw(sf::RenderWindow& win);
    sf::FloatRect getBounds();
};
#endif