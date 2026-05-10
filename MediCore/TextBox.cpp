#include "TextBox.h"
#include "Utils.h"

static sf::Color TB_BG(15, 40, 80);
static sf::Color TB_ACTIVE(50, 130, 210);
static sf::Color TB_IDLE(60, 80, 120);
static sf::Color TB_TEXT(220, 235, 255);
static sf::Color TB_LABEL(150, 185, 220);

TextBox::TextBox() : bufLen(0), maxLen(50), active(false), isPassword(false), hasLabel(false) { buffer[0] = '\0'; }

TextBox::TextBox(float x, float y, float w, float h, sf::Font& font, int ml, bool pw)
    : bufLen(0), maxLen(ml), active(false), isPassword(pw), hasLabel(false) {
    buffer[0] = '\0';
    box.setSize(sf::Vector2f(w, h)); box.setPosition(x, y);
    box.setFillColor(TB_BG); box.setOutlineThickness(1); box.setOutlineColor(TB_IDLE);
    display.setFont(font); display.setCharacterSize(14);
    display.setFillColor(TB_TEXT); display.setPosition(x + 8, y + (h - 18) / 2);
}

void TextBox::setLabel(const char* text, sf::Font& font) {
    hasLabel = true;
    labelText.setFont(font); labelText.setString(text);
    labelText.setCharacterSize(13); labelText.setFillColor(TB_LABEL);
    sf::Vector2f pos = box.getPosition();
    labelText.setPosition(pos.x, pos.y - 20);
}

void TextBox::setActive(bool a) { active = a; }
bool TextBox::getActive() { return active; }
const char* TextBox::getText() { return buffer; }

void TextBox::clear() { bufLen = 0; buffer[0] = '\0'; updateDisplay(); }

void TextBox::handleEvent(sf::Event& e) {
    if (e.type == sf::Event::MouseButtonPressed) {
        active = box.getGlobalBounds().contains((float)e.mouseButton.x, (float)e.mouseButton.y);
        box.setOutlineColor(active ? TB_ACTIVE : TB_IDLE);
    }
    if (!active) return;
    if (e.type == sf::Event::TextEntered) {
        if (e.text.unicode == '\b') { if (bufLen > 0) buffer[--bufLen] = '\0'; }
        else if (e.text.unicode >= 32 && e.text.unicode < 127 && bufLen < maxLen - 1)
        {
            buffer[bufLen++] = (char)e.text.unicode; buffer[bufLen] = '\0';
        }
        updateDisplay();
    }
}

void TextBox::updateDisplay() {
    if (isPassword) {
        char s[256]; for (int i = 0; i < bufLen; i++) s[i] = '*'; s[bufLen] = '\0';
        display.setString(s);
    }
    else display.setString(buffer);
}

void TextBox::draw(sf::RenderWindow& win) {
    if (hasLabel) win.draw(labelText);
    win.draw(box); win.draw(display);
}

sf::FloatRect TextBox::getBounds() { return box.getGlobalBounds(); }