#include <iostream>
#include "window.h"
#include "util.h"

int main(int argc, char *argv[])
{
    Window window(1024, 768);

    while (window.loop()) {

    }

    return 0;
}
