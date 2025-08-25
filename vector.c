#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int err;
extern FILE *wyjscie;

void vector_init(Vector *vec) {
    if(err) return;
    
    vec->size = 0;
    vec->capacity = 2;
    vec->data = (int *)malloc(vec->capacity * sizeof(int));
    if ((vec->data) == NULL) {
        fprintf(stderr, "ERROR 1 : brak miejsca w pamięci\n");
        err = 1;
        return;
    }
}

void vector_free(Vector *vec) {    
    if(vec->data != NULL)
        free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

void vector_resize(Vector *vec, size_t new_capacity) {
    if(err) return;
    
    if(vec->data == NULL){
        vector_free(vec);
        vector_init(vec);
    }
    
    if(vec->size == 0){
        vector_init(vec);
    }
    
    vec->data = (int *)realloc(vec->data, new_capacity * sizeof(int));
    if ((vec->data) == NULL) {
        fprintf(stderr, "ERROR 2 : brak miejsca w pamięci\n");
        err = 1;
        return;
    }
    vec->capacity = new_capacity;
}

void vector_push_back(Vector *vec, int watosc) {
    if(err) return;
    
    if (vec->size >= vec->capacity) {
        vector_resize(vec, vec->capacity * 2);
    }
    vec->data[vec->size++] = watosc;
}

size_t vector_size(Vector *vec) {
    return vec->size;
}

void vector_reverse(Vector *vec) {
    if(err) return;
    
    if(vec->size == 0) return;
    
    size_t left = 0;
    size_t right = vec->size - 1;
    
    while (left < right) {
        int temp = vec->data[left];
        vec->data[left] = vec->data[right];
        vec->data[right] = temp;
        
        left++;
        right--;
    }
}

void vector_copy(Vector *vec1, const Vector *vec2) {
    if(err) return;
    
    if (vec1->data != NULL) {
        free(vec1->data);
    }

    vec1->data = (int *)malloc(vec2->capacity * sizeof(int));
    if (!vec1->data) {
        fprintf(stderr, "ERROR 5 : brak miejsca w pamięci\n");
        err = 1;
        return;
    }

    memcpy(vec1->data, vec2->data, vec2->size * sizeof(int));
    vec1->size = vec2->size;
    vec1->capacity = vec2->capacity;
}

void ustawrozm(Vector *vec, size_t size) {
    vec->data = (int *)malloc(size * sizeof(int));
    if (!vec->data) {
        fprintf(stderr, "ERROR 9 : brak miejsca w pamięci\n");
        err = 1;
        return;
    }
    vec->size = size;
    vec->capacity = size;
    memset(vec->data, 0, size * sizeof(int));
}

void vector_copy_range(Vector *dest, const Vector *src, size_t pocz, size_t kon) {
    if(err) return;
    
    ustawrozm(dest, kon - pocz);
    for (size_t i = pocz; i < kon; i++) {
        dest->data[i - pocz] = src->data[i];
    }
}

void wypisz(Vector *wektor) {
    for (size_t i = 0; i < wektor->size; i++) {
        int wyp = wektor->data[i];
        if(wyp < 10){
            putc(wyp + '0', wyjscie);
        } else {
            putc(wyp + 'A' - 10, wyjscie);
        }
    }
    putc('\n', wyjscie);
    fflush(wyjscie);
}

void rozszerz(Vector *vec, size_t new_size) {
    if(err) return;
    
    if (new_size <= vec->size) {
        return;
    }

    int *new_data = (int *)malloc(new_size * sizeof(int));
    if (!new_data) {
        fprintf(stderr, "ERROR 10 : Brak miejsca w pamięci\n");
        err = 1;
        return;
    }

    size_t ilezer = new_size - vec->size;
    memset(new_data, 0, ilezer * sizeof(int));
    memcpy(new_data + ilezer, vec->data, vec->size * sizeof(int));

    if(vec->data != NULL) free(vec->data);
    vec->data = new_data;
    vec->size = new_size;
    vec->capacity = new_size;
}

void usunzera(Vector *wyn) {
    size_t ilezer = 0;
    while (ilezer < wyn->size - 1 && wyn->data[ilezer] == 0) {
        ilezer++;
    }

    if (ilezer > 0) {
        memmove(wyn->data, wyn->data + ilezer, (wyn->size - ilezer) * sizeof(int));
        wyn->size -= ilezer;
    }
}

int porbezzer(Vector *a, Vector *b) {
    if(a->size > b->size) return 1;
    if(a->size < b->size) return 0;
    
    for(size_t i = 0; i < a->size; i++){
        if(a->data[i] > b->data[i]) return 1;
        if(a->data[i] < b->data[i]) return 0;
    }
    return 1;
}
