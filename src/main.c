#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include "console.h"
#include <stdlib.h>
#include "unicode.h"

int main(int argc, const char **argv)
{   
    SetConsoleOutputCP(CP_UTF8);
    // argv with wide character support
    LPWSTR * uargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    parse_args(argc, uargv);
    return 0;
}