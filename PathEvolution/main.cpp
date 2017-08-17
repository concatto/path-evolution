#include <iostream>
#include "window.h"
#include "util.h"

int main(int argc, char *argv[])
{
    Window window(800, 600);

    window.loop();

    return 0;
}
