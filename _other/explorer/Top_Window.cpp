#include "Top_Window.h"
#include "Button.h"
#include "System.h"
#pragma warning(disable : 4996)

Top_Window::Top_Window(int x, int y, int W, int H) {
    this->x = x; this->y = y; this->W = W; this->H = H;

    font.loadFromFile("font.ttf");
    Text text("", font, text_size);
    text.setColor(text_color);
    this->text = text;
}

void Top_Window::draw_path(std::vector<string>& data, RenderTarget& window, string &path, Vector2i mouse_pos, int &scroll) {
    text.setString(path);
    draw(window, text, 30, 3);

    if (back.draw(window, 3, 0, mouse_pos)) {
        string buf = path;
        for (int i = 0; i < buf.size(); i++) {
            if (path.back() == '\\') { path.pop_back(); break; }
            else path.pop_back();
        }
        if (path.size() == 0) path = buf;
        else { getData(data, path); scroll = 0; }
    }

}