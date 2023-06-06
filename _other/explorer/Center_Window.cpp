#include "Center_Window.h"
#include "SFML/Graphics.hpp"
#include <string>
#include <Windows.h>
#include "System.h"
#include <vector>

#include <iostream>
#define cout std::cout
#define endl std::endl


using namespace ::sf;
typedef std::string string;
#pragma warning(disable : 4996)

Center_Window::Center_Window(int x, int y, int W, int H) {
    this->x = x; this->y = y; this->W = W; this->H = H;
    
    font.loadFromFile("font.ttf");
    Text text("", font, text_size);
    text.setColor(text_color);
    this->text = text;
}

Center_Window::File::File() {
    f.loadFromFile("ico\\folder.png");
    e.loadFromFile("ico\\exe.png");
    i.loadFromFile("ico\\photo.png");
    t.loadFromFile("ico\\txt.png");
    n.loadFromFile("ico\\noname.png");

    folder.setTexture(f);
    folder.setColor(file_color);
    exe.setTexture(e);
    exe.setColor(file_color);
    img.setTexture(i);
    img.setColor(file_color);
    txt.setTexture(t);
    txt.setColor(file_color);
    noname.setTexture(n);
    noname.setColor(file_color);
}

void Center_Window::File::folder_click(std::vector<string>& data, string& path_pos, string name, int& scroll, string& last_path) {
    if (clock() - double_click_timer <= 500 && last_name == name) {
        path_pos += '\\';
        path_pos += name;
        getData(data, path_pos);
        double_click_timer = 0;
        scroll = 0;
        last_path.clear();
    }
    double_click_timer = clock();
    last_name = name;
}
void Center_Window::File::file_click(string& path_pos, string type, string name) {
    if (clock() - double_click_timer <= 500 && last_name == name) {
        StartProgramm(path_pos + "\\" + name);
        double_click_timer = 0;
    }
    double_click_timer = clock();
    last_name = name;
}

void Center_Window::File::draw_file(Center_Window& center, RenderWindow& window, Vector2i mouse_pos,
    std::vector<string>& data, string& path_pos, int i, string s_data, int& scroll, string& last_path) {
    Sprite s;
    string t = "";
    string date = "";
    string size = "";
    string type = "";

    if (s_data[0] == '>') {
        center.text.setString("Эта папка пуста.");
        center.draw(window, center.text, 300, 10);
        return;
    }
    if (s_data[0] == '?') {
        center.text.setString("Похоже, такой папки не существет.");
        center.draw(window, center.text, 200, 10);
        return;
    }

    if (s_data[0] == '<') { // get type
        t = s_data;
        s = folder; t.erase(t.begin());
        bool b = 0;
        for (;;) {
            if (t.size() > 50) { t.pop_back(); b = 1; }
            else { if (b) t += "..."; break; }
        }
        type = "folder";
    }
    else {
        string name = "";

        int n = 0;
        for (char c : s_data) {
            if (c == '|') { n++; continue; }
            if (n == 0) name += c;
            if (n == 1) date += c;
            if (n == 2) size += c;
        }
        bool b = 0;
        for (;;) {
            if (name.size() > 32) { name.pop_back(); b = 1; }
            else { if (b)  name += "..."; break; }
        }

        t = name;

        type = getType(name);
        if (type == "exe") { s = exe; }
        else if (type == "txt") { s = txt; }
        else if (type == "png" || type == "jpg" || type == "bmp") { s = img; }
        else { s = noname; }

    }

    int mx = center.getmousepos(mouse_pos).x;
    int my = center.getmousepos(mouse_pos).y;
    s.setScale(1, 1);
    s.setColor(file_color);

    if (mx > 0 && mx < 525 && my > 0 && my < center.H) { // click
        if (i == my / 50) {
            s.setColor(Color(255, 255, 255));
            center.text.setColor(Color(255, 255, 255));
            center.text.setStyle(sf::Text::Bold | sf::Text::Italic);
            s.setScale(1.15, 1.15);

            if (GetKeyState(VK_LBUTTON) < 0 && !click) {
                last_path.clear();

                if (type == "folder") {
                    last_path = '<';
                    last_path += path_pos + "\\" + t;

                    folder_click(data, path_pos, t, scroll, last_path); 
                }
                else { 
                    last_path += path_pos + "\\" + t;
                    file_click(path_pos, type, t); 
                }
                click = 1;
            }
            if (GetKeyState(VK_LBUTTON) >= 0) click = 0;
        }
    }

    if (GetKeyState(VK_LBUTTON) < 0) {  //reset last_path
        if (mx < 0 || mx > 525 || my < 0 || my > center.H || my / 50 >= data.size() - scroll) {
            if (mx < center.W - 50)
                last_path.clear();
        }
    }


    if (data.size() > 9) {  // lift :)
        int rec_w = float(9 / float(data.size() + 2)) * center.H;
        sf::RectangleShape rectangle(sf::Vector2f(20, rec_w));
        rectangle.setFillColor(Color(255, 255, 255, 13));
        rectangle.setPosition(center.W - 20, center.y + float(9 / float(data.size() + 2)) * 50 * scroll);
        window.draw(rectangle);

        if (mx > center.W - 50 && mx < center.W && my > 0 && my < center.H) {
            if (GetKeyState(VK_LBUTTON) < 0) {
                float kof = my / float(center.H);
                int n = float(data.size() + 2) * kof;
                scroll = n - 4;

                if (scroll > int(data.size()) - 8) scroll = int(data.size()) - 8 + 1;
                if (scroll < 0) scroll = 0;
            }
        }
    }

    center.text.setString(t);
    center.draw(window, s, 10, i * 50 + 10);
    center.draw(window, center.text, 48, i * 50 + 10);

    center.text.setColor(center.text_color);
    center.text.setStyle(sf::Text::Regular);

    center.text.setString(date);
    center.draw(window, center.text, 525, i * 50 + 10);

    center.text.setString(size);
    center.draw(window, center.text, 670, i * 50 + 10);
}