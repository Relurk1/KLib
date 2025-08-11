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

Arena* new_arena() {
    Arena* arena = (Arena*)malloc(sizeof(Arena));
    arena->head = (ArenaChunk*)malloc(sizeof(ArenaChunk));
    if(!arena || !arena->head) {
        perror("Arena malloc failed");
        exit(EXIT_FAILURE);
    }
    arena->elems = 0;

    arena->head->offset = 0;
    arena->head->next = NULL;
    arena->head->buffer = (char*)mmap(0, DEFAULT_CHUNK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(arena->head->buffer == MAP_FAILED) {
        perror("Arena memory allocation failed");
        exit(EXIT_FAILURE);
    }

    return arena;
}

void* arena_alloc(Arena* arena, size_t size) {
    if(size > DEFAULT_CHUNK_SIZE) {
        fprintf(stderr, "Objects inserted into the arena can have a maximum size of %d bytes\n", DEFAULT_CHUNK_SIZE);
        exit(EXIT_FAILURE);
    }

    ArenaChunk* crr = arena->head;

    if(crr->offset + size > DEFAULT_CHUNK_SIZE) {
        if(crr->next != NULL) {
            crr = crr->next;
        }
        else {
            ArenaChunk* new_chunk = (ArenaChunk*)malloc(sizeof(ArenaChunk));
            if (!new_chunk) {
                perror("Chunk malloc failed");
                exit(EXIT_FAILURE);
            }

            new_chunk->offset = 0;
            new_chunk->next = NULL;
            new_chunk->buffer = (char*)mmap(0, DEFAULT_CHUNK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (new_chunk->buffer == MAP_FAILED) {
                perror("Chunk mmap failed");
                exit(EXIT_FAILURE);
            }

            crr->next = new_chunk;
            crr = new_chunk;
        }

    }

    uintptr_t current = (uintptr_t)(crr->buffer + crr->offset);
    uintptr_t aligned = (current + alignof(max_align_t) - 1) & ~(uintptr_t)(alignof(max_align_t) - 1);
    size_t padding = aligned - current;

    crr->offset += padding;

    void* dest = crr->buffer + crr->offset;
    crr->offset += size;

    arena->elems += 1;
    return dest;
}

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