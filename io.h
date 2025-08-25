// io.h
#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "vector.h"

int getc2(FILE *wejscie);
void czyt_liczbe(int podstawa, char pierwsza, Vector *vec);
char *dopiszout(char *sciezka);

#endif
