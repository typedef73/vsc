#pragma once
#include "Virtual_Window.h"
#include <string>
#include <vector>
#include "Button.h"

typedef std::string string;

class Top_Window : public Virtual_Window {
    int text_size = 14;
    Font font;
    Text text;
    Color text_color = Color(0, 255, 0, 175);
    Button back = Button("ico\\back.png");
public:
	Top_Window(int x, int y, int W, int H);

    void draw_path(std::vector<string>& data, RenderTarget& windows, string &path, Vector2i mouse_pos, int& scroll);
};