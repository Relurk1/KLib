#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma once

typedef struct {
    char* buffer;
    size_t bufferSize;
    size_t size;
} String;

// ##########################################################
//                     Utility Functions
// ##########################################################

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
 * @brief    Returns the character at index within the internal buffer of str. 
             Exits with code 1 if str is NULL. 
             Exits with code 1 if index >= str->size
 * @param    str - The String for which the character at index is requested
 * @param    index - a location within the buffer of str
 * @returns  str->buffer
 */
char string_get(String *str, unsigned long index) {
    if(str == NULL) {
        fprintf(stderr, "argument str cannot be NULL\n");
        exit(1);
    }
    if(index >= str->size) {
        fprintf(stderr, "argument index must be in the range [0, str->size)\n");
        exit(1);
    }
    return str->buffer[index];
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
        str->bufferSize = s*sizeof(char)+1;
        str->buffer = (char*)realloc(str->buffer, str->bufferSize);
    }
    memset(str->buffer, '\0', str->bufferSize);
    strcpy(str->buffer, src);
    str->size = s;
};

/*
 * @brief    Prints the buffer of str to stdout 
 * @param    str - the String that has its buffer printed
 * @returns  none
 */
void print_string(String* str) {
    if(str == NULL) 
        return;
    printf("%s\n", str->buffer);
}

/*
 * @brief    Dumps all the contents of str to stdout.
 * @param    str - the String that has its contents printed
 * @returns  none
 */
void dump_string(String* str) {
    if(str == NULL) 
        return;
    printf("buffer: %s\n", str->buffer);
    printf("buffer size: %lu\n", str->bufferSize);
    printf("string size: %lu\n", str->size);
}

// ##########################################################
//                     Copying Functions
// ##########################################################

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
        dest->bufferSize = s*sizeof(char)+1;
        dest->buffer = (char*)realloc(dest->buffer, dest->bufferSize);
    }
    memset(dest->buffer, '\0', dest->bufferSize);
    strcpy(dest->buffer, src);
    dest->size = s;
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
    string_copy_c(src, dest->buffer);
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
    size_t s = strlen(src);
    if(dest == NULL) {
        fprintf(stderr, "argument dest cannot be NULL\n");
        exit(1);
    }
    if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    if(num > s) {
        fprintf(stderr, "num cannot be greater than strlen(src)\n");
        exit(1);
    }

    if(num >= dest->bufferSize) {
        dest->bufferSize = num*sizeof(char)+1;
        dest->buffer = (char*)realloc(dest->buffer, dest->bufferSize);
    }
    memset(dest->buffer, '\0', dest->bufferSize);
    strncpy(dest->buffer, src, num);
    dest->size = num;
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
    string_n_copy_c(dest, src->buffer, num);
};



// ##########################################################
//                  Concatenation Functions
// ##########################################################

/*
 * @brief    Appends the buffer from src to the buffer of dest. 
             Exits with code 1 if either dest or src are NULL
 * @param    dest - the destination String that has its buffer appended to
 * @param    src - the source c-style string. src is added to the end of the buffer of dest.
 * @returns  none
 */
void string_append_c(String* dest, char* src) {
    size_t s = strlen(src);
    if(dest == NULL) {
        fprintf(stderr, "argument dest cannot be NULL\n");
        exit(1);
    }
    if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    
    if(dest->size+s >= dest->bufferSize) {
        dest->bufferSize = (dest->size+s)*sizeof(char)+1;
        dest->buffer = (char*)realloc(dest->buffer, dest->bufferSize);
    }
    strcat(dest->buffer, src);
    dest->size += s;
};

/*
 * @brief    Appends the buffer from the buffer of src to the buffer of dest. 
             Exits with code 1 if either dest or src are NULL
 * @param    dest - the destination String that has its buffer appended to
 * @param    src - the source String. the buffer of src is added to the end of the buffer of dest.
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
    string_append_c(dest, src->buffer);
};

/*
 * @brief    Appends the first num characters from src to the buffer of dest.  
             Exits with code 1 if either dest or src are NULL. 
             Exits with code 1 if num is greater than strlen(src)
 * @param    dest - the destination String that has its buffer appended
 * @param    src - the source c-style string. The buffer of dest is changed to match num chars from src exactly
 * @returns  none
 */
void string_n_append_c(String* dest, char* src, size_t num) {
    size_t s = strlen(src);
    if(dest == NULL) {
        fprintf(stderr, "argument dest cannot be NULL\n");
        exit(1);
    }
    if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    if(num > s) {
        fprintf(stderr, "num cannot be greater than strlen(src)\n");
        exit(1);
    }

    if((dest->size+num) >= dest->bufferSize) {
        dest->bufferSize = (dest->size+num)*sizeof(char)+1;
        dest->buffer = (char*)realloc(dest->buffer, dest->bufferSize);
    }
    strncat(dest->buffer, src, num);
    dest->size += s;
}

/*
 * @brief    Appends the first num characters from the buffer of src to the buffer of dest.  
             Exits with code 1 if either dest or src are NULL. 
             Exits with code 1 if num is greater than src->size
 * @param    dest - the destination String that has its buffer appended
 * @param    src - the source c-style string. The buffer of dest is changed to match num chars from src exactly
 * @returns  none
 */
void string_n_append(String* dest, String* src, size_t num) {
    if(dest == NULL) {
        fprintf(stderr, "argument dest cannot be NULL\n");
        exit(1);
    }
    if(src == NULL) {
        fprintf(stderr, "argument src cannot be NULL\n");
        exit(1);
    }
    if(num > src->size) {
        fprintf(stderr, "num cannot be greater than strlen(src)\n");
        exit(1);
    }
    string_n_append_c(dest, src->buffer, num);
}

// ##########################################################
//                  Searching Functions
// ##########################################################

/*
 * @brief    Finds the first occurence of c in str. 
             Exits with code 1 if str is NULL
 * @param    str - the String in which the character is seearched for
 * @param    c - the character to be located
 * @returns  The index of the first occurence of c in str, or -1 if c is not found
 */
size_t string_find_char(String* str, char c) {
    if(str == NULL) {
        fprintf(stderr, "argument str cannot be NULL\n");
        exit(1);
    }
    char* pch = strchr(str->buffer, c);
    if(pch == NULL)
        return -1;
    return pch-str->buffer+1;
}

/*
 * @brief    Splits str into tokens, along the characters specified in delimiters. 
             It is the caller's responsibility to free the returned strings appropriately. 
             Exits with code 1 if either str or delimiters is NULL
 * @param    str - String to be tokenized
 * @param    delimiters - c-style string containing all delimiters
 * @param    c - the number of tokens found
 * @returns  If a token is found, a pointer to the beginning of the token, otherwise NULL
 */
String** string_tokenize_c(String* str, const char* delimiters, unsigned int* c) {
    if(str == NULL) {
        fprintf(stderr, "argument str cannot be NULL\n");
    }
    if(delimiters == NULL) {
        fprintf(stderr, "argument delimiters cannot be NULL\n");
        exit(1);
    }

    char* temp1 = (char*)malloc(str->size*sizeof(char)+1);
    char* temp2 = (char*)malloc(str->size*sizeof(char)+1);
    strcpy(temp1, str->buffer);
    strcpy(temp2, str->buffer);

    int count = 0;
    char* pch = strtok(temp1, delimiters);
    if(pch == NULL) {
        *c = 0;
        return NULL;
    }
    while(pch != NULL) {
        count += 1;
        pch = strtok(NULL, delimiters);
    }

    String** tokens = (String**)malloc(count*sizeof(String*));
    pch = strtok(temp2, delimiters);
    for(int i=0; i<count; i++) {
        tokens[i] = new_string();
        string_copy_c(tokens[i], pch);
        pch = strtok(NULL, delimiters);
    }

    free(temp1);
    free(temp2);
    *c = count;
    return tokens;
}

/*
 * @brief    Splits str into tokens, along the characters specified in delimiters. 
             It is the user's responsibility to free the returned strings appropriately. 
             Exits with code 1 if either str or delimiters is NULL
 * @param    str - String to be tokenized
 * @param    delimiters - String containing all delimiters
 * @param    c - the number of tokens found
 * @returns  If a token is found, a pointer to the beginning of the token, otherwise NULL
 */
String** string_tokenize(String* str, String* delimiters, unsigned int* c) {
    if(str == NULL) {
        fprintf(stderr, "argument str cannot be NULL\n");
    }
    if(delimiters == NULL) {
        fprintf(stderr, "argument delimiters cannot be NULL\n");
        exit(1);
    }
    return string_tokenize_c(str, delimiters->buffer, c);
}


