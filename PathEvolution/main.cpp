#include <iostream>
#include "window.h"
#include "util.h"

int main(int argc, char *argv[])
{
    Window window(640, 480);

    window.loop();

    return 0;
}
