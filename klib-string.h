#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char* buffer;
    size_t bufferSize;
    size_t size;
} String;

// ##########################################################
//                  Internal Use Functions
// ##########################################################

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

void print_error(const char* func, const char* msg) {
    fprintf(stderr, "Error in function %s: %s\n", func, msg);
    exit(1);
}

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
    if(str == NULL) 
        print_error("free_string", "argument str cannot be NULL");
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
    if(str == NULL) 
        print_error("string_size", "argument str cannot be NULL");
    return str->size;
};

/*
 * @brief    Returns the internal buffer of the string as a character array. 
             Exits with code 1 if str is NULL
 * @param    str - The String for which the buffer is requested
 * @returns  str->buffer
 */
char* c_string(String *str) {
    if(str == NULL) 
        print_error("c_string", "argument str cannot be NULL");
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
    if(str == NULL) 
        print_error("string_get", "argument str cannot be NULL");
    if(index >= str->size)
        print_error("string_get", "argument index must be in the range [0, str->size)");
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
    if(str == NULL) 
        print_error("set_string", "argument str cannot be NULL");
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
    if(dest == NULL) 
        print_error("string_copy_c", "argument dest cannot be NULL");
    if(src == NULL) 
        print_error("string_copy_c", "argument src cannot be NULL");
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
    if(dest == NULL) 
        print_error("string_copy", "argument dest cannot be NULL");
    if(src == NULL) 
        print_error("string_copy", "argument src cannot be NULL");
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
    if(dest == NULL) 
        print_error("string_n_copy_c", "argument dest cannot be NULL");
    if(src == NULL) 
        print_error("string_n_copy_c", "argument src cannot be NULL");
    if(num > s)
        print_error("string_n_copy_c", "num cannot be greater than strlen(src)");

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
    if(dest == NULL) 
        print_error("string_n_copy", "argument dest cannot be NULL");
    if(src == NULL) 
        print_error("string_n_copy", "argument src cannot be NULL");
    if(num > src->size)
        print_error("string_n_copy", "num cannot be greater than strlen(src)");
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
    if(dest == NULL) 
        print_error("string_append_c", "argument dest cannot be NULL");
    if(src == NULL) 
        print_error("string_append_c", "argument src cannot be NULL");
    
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
    if(dest == NULL) 
        print_error("string_append", "argument dest cannot be NULL");
    if(src == NULL) 
        print_error("string_append", "argument src cannot be NULL");
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
    if(dest == NULL) 
        print_error("string_n_append_c", "argument dest cannot be NULL");
    if(src == NULL) 
        print_error("string_n_append_c", "argument src cannot be NULL");
    if(num > s)
        print_error("tring_n_append_c", "num cannot be greater than strlen(src)");

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
    if(dest == NULL) 
        print_error("string_n_append", "argument dest cannot be NULL");
    if(src == NULL) 
        print_error("string_n_append", "argument src cannot be NULL");
    if(num > src->size)
        print_error("tring_n_append", "num cannot be greater than strlen(src)");
    string_n_append_c(dest, src->buffer, num);
}

// ##########################################################
//                  Comparison Functions
// ##########################################################

/*
 * @brief    Compares str1 to str2.  
             Exits with code 1 if either str1 or str2 are NULL. 
 * @param    str1 - The first String to be compared
 * @param    str2 - the second c-style string to be compared
 * @returns  an integer indicating the relationship between the strings. 
 *           <0 indicates the first character that does not match has a lower value in ptr1 than in ptr2. 
 *            0 indicates the contents of both strings are equal. 
 *           >0 indicates the first character that does not match has a greater value in ptr1 than in ptr2. 
 */
int string_compare_c (String* str1, char* str2) {
    if(str1 == NULL)
        print_error("string_compare_c", "argument str1 cannot be NULL");
    if(str2 == NULL)
        print_error("string_compare_c", "argument str2 cannot be NULL");
    return strcmp(str1->buffer, str2);
}

/*
 * @brief    Compares str1 to str2.  
             Exits with code 1 if either str1 or str2 are NULL. 
 * @param    str1 - The first String to be compared
 * @param    str2 - the second String to be compared
 * @returns  an integer indicating the relationship between the strings. 
 *           <0 indicates the first character that does not match has a lower value in ptr1 than in ptr2. 
 *            0 indicates the contents of both strings are equal. 
 *           >0 indicates the first character that does not match has a greater value in ptr1 than in ptr2. 
 */
int string_compare (String* str1, String* str2) {
    if(str1 == NULL)
        print_error("string_compare", "argument str1 cannot be NULL");
    if(str2 == NULL)
        print_error("string_compare", "argument str2 cannot be NULL");
    return strcmp(str1->buffer, str2->buffer);
}

/*
 * @brief    Checks if str1 equals str2.  
             Exits with code 1 if either str1 or str2 are NULL. 
 * @param    str1 - The first String to be compared
 * @param    str2 - the second c-style string to be compared
 * @returns  1 if the strings are equal, 0 otherwise.
 */
int string_equal_c (String* str1, char* str2) {
    if(str1 == NULL)
        print_error("string_compare", "argument str1 cannot be NULL");
    if(str2 == NULL)
        print_error("string_compare", "argument str2 cannot be NULL");
    return !strcmp(str1->buffer, str2);
}

/*
 * @brief    Checks if str1 equals str2.  
             Exits with code 1 if either str1 or str2 are NULL. 
 * @param    str1 - The first String to be compared
 * @param    str2 - the second String to be compared
 * @returns  1 if the strings are equal, 0 otherwise.
 */
int string_equal (String* str1, String* str2) {
    if(str1 == NULL)
        print_error("string_compare", "argument str1 cannot be NULL");
    if(str2 == NULL)
        print_error("string_compare", "argument str2 cannot be NULL");
    return !strcmp(str1->buffer, str2->buffer);
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
    if(str == NULL) 
        print_error("string_find_char", "argument str cannot be NULL");
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
    if(str == NULL) 
        print_error("string_tokenize_c", "argument str cannot be NULL");
    if(delimiters == NULL) 
        print_error("string_tokenize_c", "argument delimiters cannot be NULL");

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
    if(str == NULL)
        print_error("string_tokenize", "argument str cannot be NULL");
    if(delimiters == NULL)
        print_error("string_tokenize", "argument delimiters cannot be NULL");
    return string_tokenize_c(str, delimiters->buffer, c);
}

