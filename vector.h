// vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Vector;

void vector_init(Vector *vec);
void vector_free(Vector *vec);
void vector_resize(Vector *vec, size_t new_capacity);
void vector_push_back(Vector *vec, int watosc);
size_t vector_size(Vector *vec);
void vector_reverse(Vector *vec);
void vector_copy(Vector *vec1, const Vector *vec2);
void ustawrozm(Vector *vec, size_t size);
void vector_copy_range(Vector *dest, const Vector *src, size_t pocz, size_t kon);
void wypisz(Vector *wektor);
void rozszerz(Vector *vec, size_t new_size);
void usunzera(Vector *wyn);
int porbezzer(Vector *a, Vector *b);

#endif
