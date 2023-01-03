#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "winstr.h"


#ifdef _WIN32
char *strndup( const char *s1, size_t n)
{
    char *copy = calloc( n+1, sizeof(char) );
    memcpy( copy, s1, n );
    return copy;
};
#endif