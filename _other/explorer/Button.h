#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>

using namespace::sf;
typedef std::string string;

class Button {
	Texture t;
	Sprite s;
	Color c = Color(255, 255, 255, 175);
	int w, h;
	bool last = 0;
public:
	Button(string sprite);

	bool draw(RenderTarget& window, int x, int y, Vector2i mouse_pos);
};