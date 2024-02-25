#include <iostream>
#include "game.hpp"

int main(int argc, char* argv[]){
    const int FPS=60;
    const int frame_delay=1000/FPS;

    Game game;
    game.run();

    return 0;

}