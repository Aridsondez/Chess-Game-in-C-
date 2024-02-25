#pragma once

class Game{
    public:
        Game();

        void init(int x, int y, int w, int h, const  char title[11]);
        void run();
        void update();
        void render();
        void clean();

        void textures();
    private:
        int screen_width;
        int screen_height;
        int game_state;//game is off 0 game is on

};