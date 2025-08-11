#pragma once

#include <stdio.h>
#include <stdalign.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define DEFAULT_CHUNK_SIZE 65536

typedef struct ArenaChunk ArenaChunk;

typedef struct ArenaChunk{
    char *buffer;
    size_t offset;
    ArenaChunk* next;
} ArenaChunk;

typedef struct {
    ArenaChunk* head;
    size_t elems;
} Arena;

Arena* new_arena();
void* arena_alloc(Arena* arena, size_t size);
void arena_reset(Arena* arena);
void arena_free(Arena* arena);
ArenaChunk* new_chunk();




// ##########################################################
//                  Internal Use Functions
// ##########################################################

ArenaChunk* new_chunk() {
    ArenaChunk* new = (ArenaChunk*)malloc(sizeof(ArenaChunk));
    if(!new) {
        perror("Chunk malloc failed");
        exit(EXIT_FAILURE);
    }
    new->offset = 0;
    new->next = NULL;
    new->buffer = (char*)mmap(0, DEFAULT_CHUNK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(new->buffer == MAP_FAILED) {
        perror("Arena memory allocation failed");
        exit(EXIT_FAILURE);
    }

    return new;
}

// ##########################################################
//                  External Functions
// ##########################################################

/*
 * @brief    Creates a new Arena
 * @param    none
 * @returns  A pointer to a new Arena
 */
Arena* new_arena() {
    Arena* arena = (Arena*)malloc(sizeof(Arena));
    if(!arena) {
        perror("Arena malloc failed");
        exit(EXIT_FAILURE);
    }
    arena->elems = 0;

    arena->head = new_chunk();

    return arena;
}

/*
 * @brief    Allocates space for an object in the arena
 * @param    arena - the Arena in which the object is being allocated
 * @returns  A pointer to the address of the allocated object
 */
void* arena_alloc(Arena* arena, size_t size) {
    if(size > DEFAULT_CHUNK_SIZE) {
        fprintf(stderr, "Objects inserted into the arena can have a maximum size of %d bytes\n", DEFAULT_CHUNK_SIZE);
        return NULL;
    }

    ArenaChunk* crr = arena->head;

    if(crr->offset + size > DEFAULT_CHUNK_SIZE) {
        if(crr->next != NULL) {
            crr = crr->next;
        }
        else {
            crr->next = new_chunk();
            crr = crr->next;
        }

    }

    uintptr_t current = (uintptr_t)(crr->buffer + crr->offset);
    uintptr_t aligned = (current + alignof(max_align_t) - 1) & ~(uintptr_t)(alignof(max_align_t) - 1);
    size_t padding = aligned - current;

    crr->offset += padding;

    if(crr->offset + size > DEFAULT_CHUNK_SIZE) {
        if(crr->next != NULL) {
            crr = crr->next;
        }
        else {
            crr->next = new_chunk();
            crr = crr->next;
        }
        return crr->buffer;
    }
    
    void* dest = crr->buffer + crr->offset;
    crr->offset += size;

    arena->elems += 1;
    return dest;
}

/*
 * @brief    Resets the arena
 * @param    arena - the arena being reset
 * @returns  none
 */
void arena_reset(Arena* arena) {
    arena->elems = 0;
    ArenaChunk *current = arena->head;
    current->offset = 0;
    current = current->next;
    while(current->next != NULL) {
        current->offset = 0;
        current = current->next;
    }
}

/*
 * @brief    Frees the arena and consequently all elements within - the arena cannot be used again after this is called
 * @param    arena - the arena being freed
 * @returns  A pointer to a new Arena
 */
void arena_free(Arena* arena) {
    ArenaChunk *current = arena->head, *next = current->next;
    munmap((void*)current->buffer, DEFAULT_CHUNK_SIZE);
    free(current);
    while (next != NULL) {
        current = next;
        next = current->next;
        munmap((void*)current->buffer, DEFAULT_CHUNK_SIZE);
        free(current);
    }
    free(arena);
}