// operations.h
#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "vector.h"

void dodaj(Vector *a, Vector *b, int podstawa, Vector *wyn);
void odejmij(Vector *vec1, Vector *vec2, Vector *wyn, int podstawa, int uszera);
void karacuba(Vector *vec1, Vector *vec2, Vector *wyn, int podstawa);
void szybkie_potegowanie(Vector *a, Vector *b, int podstawa, Vector *wyn);
void podziel(Vector *a, Vector *b, int podstawa, Vector *wyn, Vector *reszta);
void dzialanie(Vector *a, Vector *b, char symbol, int podstawa, Vector *wyn);
void zmien_podstawe(Vector *vec1, int sys1, int sys2);
int porownaj(Vector *a, Vector *b, size_t przesb);
void odejmij_na_poz(Vector *a, Vector *b, size_t poz, int podstawa, Vector *wyn);

#endif
