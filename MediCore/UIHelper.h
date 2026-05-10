#ifndef UIHELPER_H
#define UIHELPER_H
#include <SFML/Graphics.hpp>
#include "Utils.h"

namespace Color {
    static sf::Color BG(10, 25, 50);
    static sf::Color PANEL(18, 45, 85);
    static sf::Color HEADER(25, 65, 120);
    static sf::Color TITLE(200, 220, 255);
    static sf::Color TEXT_COLOR(210, 225, 245);
    static sf::Color SUBTEXT(130, 165, 205);
    static sf::Color ACCENT(70, 150, 230);
    static sf::Color SUCCESS(100, 210, 160);
    static sf::Color ERROR_C(220, 100, 100);
    static sf::Color ROW_ALT(20, 50, 95);
    static sf::Color DIVIDER(40, 75, 130);
}

inline sf::Text makeText(const char* str, sf::Font& font,
    int size, sf::Color col, float x, float y) {
    sf::Text t;
    t.setFont(font); t.setString(str);
    t.setCharacterSize(size); t.setFillColor(col);
    t.setPosition(x, y);
    return t;
}

inline void drawBackground(sf::RenderWindow& win) {
    sf::RectangleShape bg(sf::Vector2f(900, 620));
    bg.setFillColor(Color::BG); win.draw(bg);
}

inline void drawHeader(sf::RenderWindow& win, sf::Font& f,
    const char* title, const char* sub = "") {
    sf::RectangleShape bar(sf::Vector2f(900, 70));
    bar.setFillColor(Color::HEADER); win.draw(bar);
    win.draw(makeText(title, f, 22, Color::TITLE, 30, 12));
    if (sub[0]) win.draw(makeText(sub, f, 13, Color::SUBTEXT, 30, 42));
}

inline void drawMessage(sf::RenderWindow& win, sf::Font& f,
    const char* msg, bool isErr = false) {
    if (!msg[0]) return;
    sf::RectangleShape box(sf::Vector2f(860, 36));
    box.setPosition(20, 555);
    box.setFillColor(isErr ? sf::Color(60, 20, 20) : sf::Color(15, 55, 35));
    box.setOutlineThickness(1);
    box.setOutlineColor(isErr ? Color::ERROR_C : Color::SUCCESS);
    win.draw(box);
    win.draw(makeText(msg, f, 13, isErr ? Color::ERROR_C : Color::SUCCESS, 30, 562));
}

inline void drawTableHeader(sf::RenderWindow& win, sf::Font& f,
    const char* cols[], float xs[], int n, float y) {
    sf::RectangleShape row(sf::Vector2f(860, 28));
    row.setPosition(20, y); row.setFillColor(Color::HEADER); win.draw(row);
    for (int i = 0; i < n; i++) win.draw(makeText(cols[i], f, 12, Color::ACCENT, xs[i], y + 7));
}

inline void drawTableRow(sf::RenderWindow& win, sf::Font& f,
    const char* vals[], float xs[], int n, float y, bool alt = false) {
    sf::RectangleShape row(sf::Vector2f(860, 26));
    row.setPosition(20, y); row.setFillColor(alt ? Color::ROW_ALT : Color::PANEL); win.draw(row);
    for (int i = 0; i < n; i++) win.draw(makeText(vals[i], f, 12, Color::TEXT_COLOR, xs[i], y + 6));
}

#endif