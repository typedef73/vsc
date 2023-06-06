#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <Windows.h>
#include "System.h"

#include "Center_Window.h"
#include "Top_Window.h"
#include "Right_Window.h"

#include "label.h"

#pragma warning(disable : 4996)

#include <fstream>

typedef std::string string;
typedef std::wstring wstring;

#define cout std::cout
#define endl std::endl

using namespace ::sf;

const int W = 1024, H = 576;

int main() {
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    RenderWindow window(VideoMode(W, H), L"Server", Style::Close);
    window.setFramerateLimit(60);

    Texture f;
    f.loadFromFile("fon.png");
    
    Sprite fon;
    fon.setTexture(f);   
    fon.setColor(Color (255, 255, 255, 75));

    string path_pos = "C:";
    std::vector<string> data;

    getData(data, path_pos);    

    /////////////////////////    
    int otstup_1 = 25;
    int otstup_2 = 250;
    int otstup_3 = 100;

    Center_Window center(0, otstup_1, W - otstup_2, H - (otstup_1 + otstup_3));
    Top_Window top (0, 0, W, otstup_1);
    Right_Window right(W - otstup_2, otstup_1, otstup_2, H);
    //virtual_window down(0, H - otstup_3, W - otstup_2, otstup_3);
    //////////////////////////


    Vector2i mouse_pos;
    int scroll = 0;

    label l(100, 100, 200, 300, Color(255, 255, 255, 75), Color(0, 0, 0), 12);
    while (window.isOpen()) {
        window.clear(Color (100, 100, 100));
        window.draw(fon);
        mouse_pos = Mouse::getPosition(window);

        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::MouseWheelScrolled) {
                if (ev.mouseWheelScroll.delta == 1) if (scroll >= 1) scroll -= 1;                 
                if (ev.mouseWheelScroll.delta == -1) if (scroll <= int(data.size()) - 8) scroll += 1; 
                l.Scroll(ev.mouseWheelScroll.delta);
            }
            if (ev.type == Event::Closed) {
                window.close();
            }
        } 
		
        for (int i = scroll; i < data.size(); i++) {
            if (i - scroll > 8) break;
            center.file.draw_file(center, window, mouse_pos, data, path_pos, i - scroll, data[i], scroll, right.last_path);
        }
        
        top.draw_path(data, window, path_pos, mouse_pos, scroll);
        right.draw_data(window, mouse_pos, data, path_pos);

        center.draw_lines(window);
        top.draw_lines(window);
        right.draw_lines(window);
        //down.draw_lines(window);  

        window.display();
    }
    return 0;
}