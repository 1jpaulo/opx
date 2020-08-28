#include "unicode.h"
#include <stdio.h>
#include <windows.h>
// TODO: write docs for parameters functions
// TODO: write test functions within tests/

void bin(unsigned n) 
{ 
    unsigned i; 
    for (i = 1 << 31; i > 0; i = i / 2) 
        (n & i)? printf("1"): printf("0"); 
} 

// RFC 2781 implementation https://tools.ietf.org/html/rfc2781
unsigned short * encode_u16(unsigned int code)
{
    /*
        TODO: Set error codes for this function
        TODO: deal with words being everytime 2 16-bit words
    */
    // Note: Values between 0xD800 and 0xDFFF are 
    // specifically reserved for use with UTF-16, and don't 
    // have any characters assigned to them.
    /* 
        throughout internet people have assigned the W2
        to 0xFFFD (character replacement)
    */

    /* 
        Note: logical AND has higher order precedence than >=
        so we need to use parenthesis
    */

    unsigned short *words = (unsigned short *) malloc(2 * sizeof(unsigned short));
    
    // Values between 0xD800 and 0xDFFF are specifically 
    // reserved for use with UTF-16
    // values prior to 0x10FFFF reach Unicode limit, 
    // thus UTF-16 and related cannot encode

    // So we'll return a replacement character
    if(((code >= 0xD800) && (code <= 0xDFFF)) || code > 0x10FFFF) 
    {
        words[0] = 0xFFFD;
        words[1] = 0x0000;
        return words;
    }

    // if -lt 0x10000 will be a single 16-bit word
    if (code < 0x10000)
    {
        words[0] = code;
        words[1] = 0x0000;
    }
    else
    {
        // Because U is less than or equal to 0x10FFFF,
        //U' must be less than or equal to 0xFFFFF. That is,
        // U' can be represented in 20 bits.
        code -= 0x10000;

        // initialize two 16-bit unsigned integers (in this 
        // case a unsigned short) W1 and W2 to 0xD800 and 
        // 0xDC00 respectively
        words[0] = 0xD800;
        words[1] = 0xDC00;

        // Assign the 10 high-order bits of the 20-bit U' to
        // the 10 low-order bits of W1 and the 10 low-order 
        // bits of U' to the 10 low-order bits of W2.

        // Graphically, steps 2 through 4 look like:
        // U' = yyyyyyyyyyxxxxxxxxxx
        // W1 = 110110yyyyyyyyyy
        // W2 = 110111xxxxxxxxxx

        /* 
            D800 will have 10 zeros from right to left,
            as well as DC00, so we can just sum each with 
            its value's part of U, being U shifted 10 units 
            to the right for sum with W1

            We may logical & the Ux and Uy respectively with 
            a only 1 bits value, so we can be sure nothing
            will be [[[[[[[[------]]]]]]]]
        */
        words[1] += (unsigned short) code & 0x3FF;
        words[0] += (unsigned short) (code >> 10) & 0x3FF;

    }

    return words;
    
}

// RFC 2781 implementation https://tools.ietf.org/html/rfc2781
unsigned int decode_u16(unsigned short * words)
{
    unsigned int hexcode = 0;

    // If W1 < 0xD800 or W1 > 0xDFFF, the character value U 
    // is the value of W1. 
    // W1 -> words[0] 
    // W2 -> words[1]
    if (words[0] < 0xD800 || words[0] > 0xDFFF)
    {
        hexcode = words[0];
    }
    // Determine if W1 is between 0xD800 and 0xDBFF. If 
    // not, the sequence is in error and no valid character 
    // can be obtained using W1.
    if( !(words[0] > 0xD800 && words[0] < 0xDBFF) )
    {
        // TODO error
        return words[0];
    }
    // If there is no W2 (that is, the sequence ends with 
    // W1), or if W2 is not between 0xDC00 and 0xDFFF, the 
    // sequence is in error.
    // TODO error
    if(words[1] == 0x0000 || !(words[1] >= 0xDC00 && words[1] <= 0xDFFF))
    {
        return words[0];
    }
    
    // Construct a 20-bit unsigned integer U', taking the 10 
    // low-order bits of W1 as its 10 high-order bits and 
    // the 10 low-order bits of W2 as its 10 low-order bits.
    
    /* 
        logical AND with 0x3FF will copy only the 10 
        low-order bits of W1, then we shift it 10 bits to
        the left and pass it to hexcode
    */
    hexcode = (words[0] & 0x3FF) << 10;
    /* 
        logical OR with the the 10 low-order bits of W2, so
        we'll copy it to hexcode, since there are only 0 
        value bits because we initialized hexcode with 0.
    */
    hexcode |= (words[1] & 0x3FF);

    // Add 0x10000 to U' to obtain the character value U. 
    hexcode += 0x10000;

    return hexcode;
}

// RFC 3629 implementation https://tools.ietf.org/html/rfc3629
unsigned char * encode_u8(unsigned int code)
{
    // TODO: refactor this peace of code
    short len;
    if      (code < 0x80)   len = 1;
    else if (code < 0x800)  len = 2;
    else if (code < 0x1000) len = 3;
    else    len = 4;
    
    unsigned char * words = (unsigned char *) malloc( (len + 1) * sizeof(unsigned char) );

    short n_octets;

    // characters between 0xD800 and 0xDFFF are specific to
    // utf-16 encoding scheme and bigger than 0x10FFFF 
    // cannot be encoded in UTF, since isn't supported by 
    // unicode

    // Because of this, we'll return a replecement character
    if(((code >= 0xD800) && (code <= 0xDFFF)) || code > 0x10FFFF) 
    {
        words[0] = 0xEF;
        words[1] = 0xBF;
        words[2] = 0xBD;
        words[3] = '\0';
        return words;
    }

    // Determine the number of octets required 
    if (code < 0x80) 
    {
        n_octets = 1;
        words[0] = (code & 0x7f);
        words[1] = '\0';
        return words;
    }
    else if (code < 0x800)   
    {
        n_octets = 2;
        words[0] = 0xC0;
    }
    else if (code < 0x10000)
    {
        n_octets = 3;
        words[0] = 0xE0;
    }
    else
    {
        n_octets = 4;
        words[0] = 0xF0;
    }

    int i;
    /*
        Filling continuation bytes
        // 0x80 - 1000 0000
    */
    for (i = 0; i < n_octets; ++i)
        words[n_octets - i] = 0x80;

    for (i = 0; i < n_octets; i++)
    {
        // filling the lowest-order bits of every octet
        // but the first
        // 0X3F - 0011 1111
        /* 
            we'll logical AND 0x3F with the code, so the 
            six least significant bits will be the same as
            code, but the others will AND with 0, and then
            won't be computed within the octets.

        */
        words[n_octets - i - 1] |= (code & 0x3F);
        //printf("%d\n", n_octets - i - 1);

        // now, that we copied the six least significant 
        // bits, we can shift it and get the next six
        // for readability 
        // code = code >> 6 (shifting code six bits to right)
        code >>= 6;
    }
    /*
         Char. number range  |        UTF-8 octet sequence
            (hexadecimal)    |              (binary)
        --------------------+------------------------------------
        0000 0000-0000 007F | 0xxxxxxx
        0000 0080-0000 07FF | 110xxxxx 10xxxxxx
        0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
        0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        
        Observing the above table we are sure that we have to
        logical AND with words of 7, 5, 4, 3 bits valued as
        1, however, assuming we already returned the utf-8
        values with one octet only, we can begin from 1F 
        that is represented by 11111 (5 ones) and shift 
        those bits n_octets - 2 bits to the right.
    */
    words[0] |= code & (0x1f >> (n_octets - 2));

    // end multibyte string with \'0'
    words[n_octets] = 0x00;

    return words;
}

void string_u8(short unsigned int *str, unsigned char ***str_u8, size_t len)
{
    *str_u8 = (unsigned char **) malloc((len + 1) * sizeof(unsigned char *));
    
    size_t i;
    for(i = 0; i < len; i++)
        (*str_u8)[i] = encode_u8(str[i]);

    // nul-terminator of multibyte string
    (*str_u8)[len] = '\0';
}

void print_u8(unsigned char **str_u8, size_t len)
{
    size_t i;
    for(i = 0; i < len; i++)
        wprintf(L"%s", str_u8[i]);
}