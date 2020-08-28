#include "console.h"
#include <unistd.h>

int parse_args(int argc, LPWSTR *uargv)
{
    if (argc == 1) printf("Error: No arguments provided.\n");
    if (argc >= 2) 
    {
        if (wcscmp(uargv[1], L"-l") == 0) 
        {
            list_dir(uargv[2]);
        }
    }
    return 0;
}
