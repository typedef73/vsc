#include "Button.h"
#include <iostream>

Button::Button(string sprite) {
	t.loadFromFile(sprite);
	s.setTexture(t);
	w = t.getSize().x;
	h = t.getSize().y;
}

bool Button::draw(RenderTarget& window, int x, int y, Vector2i mouse_pos) {
	int mx = mouse_pos.x;
	int my = mouse_pos.y;
	bool res = 0;

	if (mx >= x && mx <= x + w && my >= y && my <= y + h) {
		s.setColor(Color(255, 255, 255, 255));

		if (GetKeyState(VK_LBUTTON) < 0 && !last) {
			res = 1;
			last = 1;
		}	
		if (GetKeyState(VK_LBUTTON) >= 0) last = 0;
	}
	else {
		s.setColor(c);
	}
	
	s.setPosition(x, y);
	window.draw(s);
	return res;
}