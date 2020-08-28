#include <wchar.h>
#include <stdlib.h>

void            bin         (unsigned);
unsigned short  *encode_u16 (unsigned int code);
unsigned char   *encode_u8  (unsigned int code);
unsigned int    decode_u16  (unsigned short * words);
void            string_u8   (short unsigned int *str, unsigned char ***str_u8, size_t len);
void            print_u8    (unsigned char **str_u8, size_t len);