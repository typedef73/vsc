#include "Right_Window.h"
#include "Button.h"
#include "System.h"
#pragma warning(disable : 4996)
#include <vector>

Right_Window::Right_Window(int x, int y, int W, int H) {
    this->x = x; this->y = y; this->W = W; this->H = H;

    font.loadFromFile("font.ttf");
    Text text("", font, text_size);
    Text text2("", font, text2_size);
    text.setColor(text_color);
    text2.setColor(text_color);
    this->text = text;
    this->text2 = text2;

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

void Right_Window::draw_data(RenderWindow& window, Vector2i mouse_pos, std::vector<string>& data, string path_pos) {
    Sprite s;
    string name = "";
    string type;

    if (last_path.size() > 0) {
        int i_name = last_path.size() - 1;
        for (int i = last_path.size() - 1; i >= 0; i--) {
            if (last_path[i] == '\\') break;
            else i_name--;
        }
        for (int i = i_name + 1; i < last_path.size(); i++) {
            name += last_path[i];
        }

        if (last_path[0] == '<') { // get type
            s = folder;
            type = "folder";
        }
        else {
            type = getType(name);
            if (type == "exe") { s = exe; }
            else if (type == "txt") { s = txt; }
            else if (type == "png" || type == "jpg" || type == "bmp") { s = img; }
            else { s = noname; }
        }

        /////////////////////////////////
        if (del_button.draw(window, x + 20, y + 125, mouse_pos)) {  // del
            string p_file = last_path;
            if (p_file[0] != '<') {
                DeleteFile(string_to_wstring(p_file.c_str()).c_str());
            }
            else {
                p_file.erase(p_file.begin());
                remove_dir(string_to_wstring(p_file).c_str());
            }
            for (;;) { if (p_file.back() != '\\') p_file.pop_back(); else { p_file.pop_back(); break; } } getData(data, p_file);
        }

        if (type == "folder") {
            if (size_button.draw(window, x + 145, y + 125, mouse_pos)) {
                last_p_size = last_path; last_p_size.erase(last_p_size.begin());
                last_size = getFolderSize(string_to_wstring(last_p_size));

            }
            string p_file = last_path; p_file.erase(p_file.begin());
            if (p_file == last_p_size) {
                text2.setString(last_size);
                draw(window, text2, 100, 40);
            }
        }


        ///////////////////////////////

        s.setScale(2, 2);
        draw(window, s, 10, 20);

        text.setString(name);
        draw(window, text, 2, 100);
    }
}