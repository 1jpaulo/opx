#include "file.h"

int listFiles(const char *originalPath)
{
    HANDLE explorer;
    WIN32_FIND_DATA foundData;
    DWORD errnum;
    char * path = (char *) originalPath;
    LARGE_INTEGER size;

    // concatenate \* wildcard at end of file path
    strcat(path, "\\*");
    explorer = FindFirstFile(path, &foundData);

    do {

        if( foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) // 16(0x10) detects if is a directory
        {
            // if is a dir, print its name
            printf("<DIR>\t%s\n", foundData.cFileName);
        } 
        else 
        {
            size.LowPart = foundData.nFileSizeLow;
            size.HighPart = foundData.nFileSizeHigh;

            printf("%lld B\t%s\n", size.QuadPart, foundData.cFileName);
        }
    
    }while( FindNextFile(explorer, &foundData) != 0 );
    
    if ( ( errnum = GetLastError() ) != ERROR_NO_MORE_FILES ) // 18 (0x12) THERE'S STILL FILES?
        printf("%ld\n", errnum);
    
    FindClose(explorer);
    
    return 0;
}
