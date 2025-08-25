#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define czytajc if(c==EOF)break;c=getc2(wejscie);if(c==EOF)break;putc(c,wyjscie);

extern int err;
extern FILE *wyjscie;
extern FILE *wejscie;
extern int ileEOL;

int getc2(FILE * wejscie){
	int a=getc(wejscie);
	if(a=='\r'){
		a=getc(wejscie);
	}
	return a;
}

// wczytanie liczby z wejścia
void czyt_liczbe(int podstawa,char pierwsza, Vector *vec){	
	if(err)return;
	
	int c=pierwsza;
	
	int cyf;
	while(c>='0' && c<='F'){
		if(c>='A' && c<='F'){
			cyf=c-'A'+10;
			
			if(cyf>=podstawa){
				fprintf(wyjscie, "\nERROR 7 : niepoprawny znak\n");
				err=1;
				return;
			}
			vector_push_back(vec,cyf);
		}else if(c>='0' && c<='9'){
			cyf=c-'0';
			
			if(cyf>=podstawa){
				fprintf(wyjscie, "\nERROR 8 : niepoprawny znak\n");
				err=1;
				return;
			}
			vector_push_back(vec,cyf);
		}else{
			fprintf(wyjscie, "\nERROR 10 : niepoprawny znak\n");
			err=1;
			return;
		}
		czytajc
	}
	if(c=='\n'){
		ileEOL=1;
	}else{
		fprintf(wyjscie, "\nERROR 9 : niepoprawny znak\n");
		err=1;
		return;
	}
}

// tworzenie wyjscia na podstawie wejscia (dopisuje .txt)
char *dopiszout(char *sciezka){
	if (!sciezka) {
		fprintf(stderr, "Niepoprawna ścieżka wejściowa\n");
		exit(EXIT_FAILURE);
	}

	size_t rozm = strlen(sciezka);
	char *out = (char *)malloc((rozm+5)*(sizeof(char)));
		
	if(!out){
		fprintf(stderr, "ERROR : brak miejsca w pamięci\n");
		return sciezka;
	}
	
	memmove(out+4*(sizeof(char)),sciezka,rozm);
	out[0]='o';
	out[1]='u';
	out[2]='t';
	out[3]='_';
	out[rozm+4]=0;
	return out;
}
