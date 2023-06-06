#include "Virtual_Window.h"
#include "SFML/Graphics.hpp"

using namespace ::sf;

Virtual_Window::Virtual_Window(){}
Virtual_Window::Virtual_Window(int x, int y, int W, int H) {
    this->x = x; this->y = y; this->W = W; this->H = H;
}

void Virtual_Window::draw_lines(sf::RenderTarget& window) {
    sf::VertexArray line(sf::Lines, 8);
    line[0].position = sf::Vector2f(x, y); line[1].position = sf::Vector2f(x, y + H);
    line[2].position = sf::Vector2f(x, y + H); line[3].position = sf::Vector2f(x + W, y + H);
    line[4].position = sf::Vector2f(x + W, y + H); line[5].position = sf::Vector2f(x + W, y);
    line[6].position = sf::Vector2f(x + W, y); line[7].position = sf::Vector2f(x, y);
    window.draw(line);
}
void Virtual_Window::draw(sf::RenderTarget& window, Sprite& s, int sx, int sy) {
    s.setPosition(x + sx, y + sy);
    window.draw(s);
}
void Virtual_Window::draw(sf::RenderTarget& window, Text& t, int sx, int sy) {
    t.setPosition(x + sx, y + sy);
    window.draw(t);
}
Vector2i Virtual_Window::getmousepos(Vector2i mouse_pos) {
    sf::Vector2i position = sf::Vector2i(mouse_pos.x - x, mouse_pos.y - y);
    return position;
}