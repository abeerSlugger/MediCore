#include "Button.h"

static sf::Color BTN_NORMAL(30, 90, 160);
static sf::Color BTN_HOVER(50, 130, 210);
static sf::Color BTN_TEXT(240, 245, 255);

Button::Button() {}

Button::Button(float x, float y, float w, float h,
    const char* text, sf::Font& font, int fontSize) {
    normalColor = BTN_NORMAL; hoverColor = BTN_HOVER;
    box.setSize(sf::Vector2f(w, h)); box.setPosition(x, y);
    box.setFillColor(normalColor); box.setOutlineThickness(1);
    box.setOutlineColor(sf::Color(100, 160, 220));
    label.setFont(font); label.setString(text);
    label.setCharacterSize(fontSize); label.setFillColor(BTN_TEXT);
    sf::FloatRect tb = label.getLocalBounds();
    label.setPosition(x + (w - tb.width) / 2 - tb.left, y + (h - tb.height) / 2 - tb.top);
}

void Button::setPosition(float x, float y) {
    box.setPosition(x, y);
    sf::Vector2f sz = box.getSize();
    sf::FloatRect tb = label.getLocalBounds();
    label.setPosition(x + (sz.x - tb.width) / 2 - tb.left, y + (sz.y - tb.height) / 2 - tb.top);
}

void Button::setText(const char* text) {
    label.setString(text);
    sf::Vector2f pos = box.getPosition(), sz = box.getSize();
    sf::FloatRect tb = label.getLocalBounds();
    label.setPosition(pos.x + (sz.x - tb.width) / 2 - tb.left, pos.y + (sz.y - tb.height) / 2 - tb.top);
}

bool Button::isClicked(sf::Event& e) {
    if (e.type == sf::Event::MouseButtonReleased)
        return box.getGlobalBounds().contains((float)e.mouseButton.x, (float)e.mouseButton.y);
    return false;
}

void Button::handleHover(sf::RenderWindow& win) {
    sf::Vector2i m = sf::Mouse::getPosition(win);
    box.setFillColor(box.getGlobalBounds().contains((float)m.x, (float)m.y) ? hoverColor : normalColor);
}

void Button::draw(sf::RenderWindow& win) {
    handleHover(win); win.draw(box); win.draw(label);
}

sf::FloatRect Button::getBounds() { return box.getGlobalBounds(); }