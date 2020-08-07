#include "console.h"

int parseArguments(int argc, const char **argv)
{
    if (argc == 3) {
        if (strcmp(argv[1], "-l") == 0) {
            listFiles(argv[2]);
        }
    }
    return 0;
}
