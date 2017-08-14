#include <iostream>
#include "window.h"
#include "util.h"

int main(int argc, char *argv[])
{
    Window window(1280, 1024);

    window.loop();

    return 0;
}
