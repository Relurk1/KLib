#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma once

typedef struct {
    char* buffer;
    unsigned long bufferSize;
    unsigned long size;
} String;

/*
 * @brief : Allocates the memory needed for the String struct, and initializes the internal buffer and the size variables
 * @param : none
 * @returns : A pointer to the newly created String struct  
 */
String* new_string() {
    String* str;
    str = (String*)malloc(sizeof(String));
    str->buffer = (char*)malloc(1*sizeof(char));
    str->buffer[0] = '\0';
    str->bufferSize = 1;
    str->size = 0;
    return str;
};

/*
 * @brief : Sets the internal buffer of the string to a specified value, overwriting anything that may have been there already
            Exits with code 1 if str is NULL
 * @param : String* str - the String that has its buffer set
 *          char* src - the source string. The buffer of str is changed to match src exactly.
 * @returns : none
 */
void set_string(String* str, const char* src) {
    if(str == NULL) {
        fprintf(stderr, "str cannot be NULL\n");
        exit(1);
    }
    unsigned long s = strlen(src);
    if(s >= str->bufferSize) {
        str->buffer = (char*)realloc(str->buffer, s*sizeof(char)+1);
        str->bufferSize = s*sizeof(char)+1;
    }
    memset(str->buffer, '\0', str->bufferSize);
    strcpy(str->buffer, src);
    str->size = s;
};

/*
 * @brief : Copies the buffer of src into dest, overwriting anything that may have been in the buffer of dest
            Exits with code 1 if either dest or src are NULL
 * @param : String* dest - the destination String that has its buffer overwritten
 *          String* src - the source String. The buffer of dest is changed to match src exactly.
 * @returns : none
 */
void string_copy(String* dest, String* src) {
    if(dest == NULL) {
        fprintf(stderr, "Destination string cannot be NULL\n");
        exit(1);
    }
    else if(src == NULL) {
        fprintf(stderr, "Source string cannot be NULL\n");
        exit(1);
    }
    if(src->size >= dest->bufferSize) {
        dest->buffer = (char*)realloc(dest->buffer, src->size*sizeof(char)+1);
        dest->bufferSize = src->size*sizeof(char)+1;
    }
    memset(dest->buffer, '\0', dest->bufferSize);
    strcpy(dest->buffer, src->buffer);
    dest->size = src->size;
}

unsigned long string_size(String *str) {
    return str->size;
};

char* c_string(String *str) {
    return str->buffer;
};

void free_string(String *str) {
    free(str->buffer);
    free(str);
};
