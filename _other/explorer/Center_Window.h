#pragma once
#include "Virtual_Window.h"
#include "SFML/Graphics.hpp"
#include <string>

using namespace ::sf;
typedef std::string string;

class Center_Window : public Virtual_Window {
    int text_size = 20;
    Font font;
    Text text;   
    Color text_color = Color(255, 255, 255, 175);
public:
    
    Center_Window(int x, int y, int W, int H);

    class File {
        Texture f, e, i, t, n;
        Sprite folder, exe, img, txt, noname;
        Color file_color = Color(255, 255, 255, 175);   
        string last_name = "";
        int double_click_timer = 0;
        bool click = 0;
      

        void folder_click(std::vector<string>& data, string& path_pos, string name, int& scroll, string& last_path);
        void file_click(string& path_pos, string type, string name);
    public:
        void draw_file(Center_Window& center, RenderWindow& window, Vector2i mouse_pos,
            std::vector<string>& data, string& path_pos, int i, string s_data, int& scroll, string& last_path);

        File();        
    } file;
};