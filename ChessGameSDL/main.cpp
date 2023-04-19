#include <iostream>
#include <SDL.h>
#include "Application.h"
//#undef main
using namespace std;

int main(int argc, char* args[]) {

    Application app;

    app.loop();
    app.draw();

    return 0;
}