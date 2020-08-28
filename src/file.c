#include "file.h"
#include <wchar.h>
#include <stdlib.h>
#include "unicode.h"

// TODO originalPath should not be const
int list_dir(wchar_t *originalPath)
{
    // TODO verify whether originalPath is absolute or not
    HANDLE explorer;
    WIN32_FIND_DATAW foundData;
    DWORD errnum;
    wchar_t * path = (wchar_t *) originalPath;
    LARGE_INTEGER size;

    // TODO prepend \\?\ to workaroung maximum path length limitation
    // https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
    
    // concatenate \* wildcard at end of file path
    wcscat(path, L"\\*");
    wprintf(L"%s\n", path);
    explorer = FindFirstFileW(path, &foundData);
    do 
    {
        if( foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) // 16(0x10) checks if it is a dir
        {
            printf("<DIR>\t%ls\n", foundData.cFileName);
        } 
        else 
        {
            size.LowPart = foundData.nFileSizeLow;
            size.HighPart = foundData.nFileSizeHigh;
            size_t len = wcslen(foundData.cFileName);
            unsigned char **s;

            string_u8(foundData.cFileName, &s, len);
            wprintf(L"%lld B\t", size.QuadPart);
            print_u8(s, len);
            printf("\n");
            
        }
    
    }while( FindNextFileW(explorer, &foundData) != 0 );
    
    if ( ( errnum = GetLastError() ) != ERROR_NO_MORE_FILES ) // 18 (0x12) is there still files?
        printf("%ld\n", errnum);
    
    FindClose(explorer);
    
    return 0;
}