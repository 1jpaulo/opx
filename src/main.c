#include <stdio.h>
#include <windows.h>

#include "console.h"

int main(int argc, const char **argv)
{
    SetConsoleOutputCP(65001);
    parseArguments(argc, argv);

    return 0;
}
