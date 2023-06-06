#pragma once
#include "Virtual_Window.h"
#include <string>
#include <vector>
#include "Button.h"

typedef std::string string;

class Right_Window : public Virtual_Window {
    int text_size = 12;
    int text2_size = 24;
    Font font;
    Text text, text2;
    Color text_color = Color(255, 255, 255, 255); 
    Color file_color = Color(255, 255, 255, 175);

    Texture f, e, i, t, n;
    Sprite folder, exe, img, txt, noname;    

    Button del_button = Button("ico\\Button_delete.png");
    Button size_button = Button("ico\\Button_size.png");
    //Button newfile_button = Button("ico\\Button_new.png");

    string last_size = "";
    string last_p_size = "";

public:    
    string last_path = "";

    Right_Window(int x, int y, int W, int H);

    void draw_data(RenderWindow& window, Vector2i mouse_pos, std::vector<string>& data, string path_pos);
};