#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma once

typedef struct {
    char* buffer;
    size_t bufferSize;
    size_t size;
} String;

/*
 * @brief    Allocates the memory needed for the String struct, and initializes the internal buffer and the size variables
 * @param    none
 * @returns  A pointer to the newly created String struct
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
 * @brief    Sets the internal buffer of the string to a specified value, overwriting anything that may have been there already. 
             Exits with code 1 if str is NULL
 * @param    str - the String that has its buffer set
 * @param    src - the source  c-style string. The buffer of str is changed to match src exactly.
 * @returns  none
 */
void set_string(String* str, const char* src) {
    if(str == NULL) {
        fprintf(stderr, "argument str cannot be NULL\n");
        exit(1);
    }
    size_t s = strlen(src);
    if(s >= str->bufferSize) {
        str->buffer = (char*)realloc(str->buffer, s*sizeof(char)+1);
        str->bufferSize = s*sizeof(char)+1;
    }
    memset(str->buffer, '\0', str->bufferSize);
    strcpy(str->buffer, src);
    str->size = s;
};

/*
 * @brief    Copies the buffer of src into dest, overwriting anything that may have been in the buffer of dest. 
             Exits with code 1 if either dest or src are NULL
 * @param    dest - the destination String that has its buffer overwritten
 * @param    src - the source String. The buffer of dest is changed to match src exactly.
 * @returns  none
 */
void string_copy(String* dest, String* src) {
    if(dest == NULL) {
        fprintf(stderr, "arguement dest cannot be NULL\n");
        exit(1);
    }
    else if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    if(src->size >= dest->bufferSize) {
        dest->buffer = (char*)realloc(dest->buffer, src->size*sizeof(char)+1);
        dest->bufferSize = src->size*sizeof(char)+1;
    }
    memset(dest->buffer, '\0', dest->bufferSize);
    strcpy(dest->buffer, src->buffer);
    dest->size = src->size;
};

/*
 * @brief    Copies src into the buffer of dest, overwriting anything that may have been in the buffer previously. 
             Exits with code 1 if either dest or src are NULL
 * @param    dest - the destination String that has its buffer overwritten
 * @param    src - the source c-style string. The buffer of dest is changed to match src exactly.
 * @returns  none
 */
void string_copy_c(String* dest, char* src) {
    if(dest == NULL) {
        fprintf(stderr, "argument dest cannot be NULL\n");
        exit(1);
    }
    if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    size_t s = strlen(src);
    if(s >= dest->bufferSize) {
        dest->buffer = (char*)realloc(dest->buffer, s*sizeof(char)+1);
        dest->bufferSize = s*sizeof(char)+1;
    }
    memset(dest->buffer, '\0', dest->bufferSize);
    strcpy(dest->buffer, src);
    dest->size = s;
};

/*
 * @brief    Copies num characters from the buffer of src into dest, overwriting anything that may have been in the buffer of dest. 
             Exits with code 1 if either dest or src are NULL. 
             Exits with code 1 if num is longer than src->size
 * @param    dest - the destination String that has its buffer overwritten
 * @param    src - the source String. The buffer of dest is changed to match num chars from the buffer of src exactly
 * @returns  none
 */
void string_n_copy(String* dest, String* src, size_t num) {
    if(dest == NULL) {
        fprintf(stderr, "argument dest cannot be NULL\n");
        exit(1);
    }
    if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    if(num > src->size) {
        fprintf(stderr, "num cannot be greater than src->size\n");
        exit(1);
    }

    if(num >= dest->bufferSize) {
        dest->buffer = (char*)realloc(dest->buffer, num*sizeof(char)+1);
        dest->bufferSize = num*sizeof(char)+1;
    }
    memset(dest->buffer, '\0', dest->bufferSize);
    strncpy(dest->buffer, src->buffer, num);
    dest->size = num;
};

/*
 * @brief    Copies num characters from src into the buffer of dest, overwriting anything that may have been in the buffer previously. 
             Exits with code 1 if either dest or src are NULL. 
             Exits with code 1 if num is longer than strlen(src)
 * @param    dest - the destination String that has its buffer overwritten
 * @param    src - the source c-style string. The buffer of dest is changed to match num chars from src exactly
 * @returns  none
 */
void string_n_copy_c(String* dest, char* src, size_t num) {
    unsigned long s = strlen(src);
    if(dest == NULL) {
        fprintf(stderr, "argument dest cannot be NULL\n");
        exit(1);
    }
    if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    if(num > s) {
        fprintf(stderr, "num cannot be greater than src->size\n");
        exit(1);
    }

    if(num >= dest->bufferSize) {
        dest->buffer = (char*)realloc(dest->buffer, num*sizeof(char)+1);
        dest->bufferSize = num*sizeof(char)+1;
    }
    memset(dest->buffer, '\0', dest->bufferSize);
    strncpy(dest->buffer, src, num);
    dest->size = num;
};

/*
 * @brief    Appends the buffer from src to the buffer of dest. 
             Exits with code 1 if either dest or src are NULL
 * @param    dest - the destination String that has its buffer overwritten
 * @param    src - the source c-style string. The buffer of dest is changed to match num chars from src exactly
 * @returns  none
 */
void string_append(String* dest, String* src) {
    if(dest == NULL) {
        fprintf(stderr, "argument dest cannot be NULL\n");
        exit(1);
    }
    if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    
    if(dest->size+src->size >= dest->bufferSize) {
        dest->buffer = (char*)realloc(dest->buffer, (dest->size+src->size)*sizeof(char)+1);
        dest->bufferSize = (dest->size+src->size)*sizeof(char)+1;
    }
    strcat(dest->buffer, src->buffer);
    dest->size += src->size;
};

/*
 * @brief    Appends the buffer from src to the buffer of dest. 
             Exits with code 1 if either dest or src are NULL
 * @param    dest - the destination String that has its buffer overwritten
 * @param    src - the source c-style string. The buffer of dest is changed to match num chars from src exactly
 * @returns  none
 */
void string_append_c(String* dest, char* src) {
    unsigned long s = strlen(src);
    if(dest == NULL) {
        fprintf(stderr, "argument dest cannot be NULL\n");
        exit(1);
    }
    if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    
    if(dest->size+s >= dest->bufferSize) {
        dest->buffer = (char*)realloc(dest->buffer, (dest->size+s)*sizeof(char)+1);
        dest->bufferSize = (dest->size+s)*sizeof(char)+1;
    }
    strcat(dest->buffer, src);
    dest->size += s;
};

/*
 * @brief    Returns the size of the string. 
             Exits with code 1 if str is NULL
 * @param    str - The String for which the size is queried
 * @returns  str->size
 */
size_t string_size(String *str) {
    if(str == NULL) {
        fprintf(stderr, "argument str cannot be NULL\n");
        exit(1);
    }
    return str->size;
};


/*
 * @brief    Returns the internal buffer of the string as a character array. 
             Exits with code 1 if str is NULL
 * @param    str - The String for which the buffer is requested
 * @returns  str->buffer
 */
char* c_string(String *str) {
    if(str == NULL) {
        fprintf(stderr, "argument str cannot be NULL\n");
        exit(1);
    }
    return str->buffer;
};

/*
 * @brief    Frees the memory allocated to str. 
             Exits with code 1 if str is NULL
 * @param    str - The String being freed
 * @returns  none
 */
void free_string(String *str) {
    if(str == NULL) {
        fprintf(stderr, "argument str cannot be NULL\n");
        exit(1);
    }
    free(str->buffer);
    free(str);
};
