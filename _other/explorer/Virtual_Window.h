#pragma once
#include "SFML/Graphics.hpp"
using namespace ::sf;

struct Virtual_Window {
    int x = 0, y = 0, W = 0, H = 0;

    Virtual_Window();
    Virtual_Window(int x, int y, int W, int H);
    void draw_lines(sf::RenderTarget& window);
    void draw(sf::RenderTarget& window, sf::Sprite& s, int sx, int sy);
    void draw(sf::RenderTarget& window, sf::Text& t, int sx, int sy);
    sf::Vector2i getmousepos(Vector2i mouse_pos);
};