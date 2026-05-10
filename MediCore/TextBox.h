#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <SFML/Graphics.hpp>

class TextBox {
    sf::RectangleShape box;
    sf::Text           display;
    sf::Text           labelText;
    char               buffer[256];
    int                bufLen, maxLen;
    bool               active, isPassword, hasLabel;
    void updateDisplay();
public:
    TextBox();
    TextBox(float x, float y, float w, float h,
        sf::Font& font, int maxLen = 50, bool password = false);
    void setLabel(const char* text, sf::Font& font);
    void setActive(bool a);
    bool getActive();
    const char* getText();
    void clear();
    void handleEvent(sf::Event& e);
    void draw(sf::RenderWindow& win);
    sf::FloatRect getBounds();
};
#endif
