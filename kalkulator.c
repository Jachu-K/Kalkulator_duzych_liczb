// kalkulator.c (zmodyfikowany)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "operations.h"
#include "io.h"

#define czytajc if(c==EOF)break;c=getc2(wejscie);if(c==EOF)break;putc(c,wyjscie);

// Zmienne globalne
FILE *wejscie;
FILE *wyjscie;
int ileEOL = 0;
int err = 0;

int main(int argc, char **argv){
	wejscie=stdin;
	wyjscie=stdout;
	if(argc>=2){
	wejscie=fopen(argv[1],"r");
	}
	if(argc>=3){
		wyjscie=fopen(argv[2],"w");
	}else if(argc>=2){
		char *output_file = dopiszout(argv[1]);
		wyjscie = fopen(output_file, "w");
		free(output_file);
		
	}
	if (!wejscie) {
		fprintf(stderr,"Nie można otworzyć pliku wejściowego");
		return 1;
	}
	if (!wyjscie) {
		fprintf(stderr,"Nie można otworzyć pliku wyjściowego");
		fclose(wejscie);
		return 1;
	}	
	int c=0;
	while(1){
		czytajc
		// W przypadku błędu kontynuujemy od następnego działania
		if(err==1){
			err=0;
			ileEOL=0;
			if(c=='\n')ileEOL++;
			while(ileEOL<3){
				czytajc
				if(c=='\n'){
					ileEOL++;
				}
			}
		}
		
		while(c=='\n'){
			czytajc
		}
		
		// Jeżeli pierwszy znak jest cyfrą - mamy do czynienia ze zmianą systemu
		if(c>='0' && c<='9'){
			
			// Wczytujemy początkowy i docelowy system liczbowy
			int sys1,sys2;
			sys1=c-'0';
			
			czytajc
			if(c>='0' && c<='9'){
				sys1*=10;
				sys1+=c-'0';
				czytajc
				
				// Jeżeli system ma 3 cyfry, jest niepoprawny (obsługujemy systemy od 2 do 16)
				if(c!=' ' && c!='\n'){
					while(c!=' ' && c!='\n'){
						czytajc;
					}
					fprintf(wyjscie,"\nError niepoprawna podstawa\n");
					err=1;
					continue;
				}					
			}
			while(c<'0' || c>'9'){
				if(c==EOF)break;	
				czytajc
			}
			sys2=c-'0';
			
			czytajc
			if(c>='0' && c<='9'){
				sys2*=10;
				sys2+=c-'0';
				czytajc
				// Jeżeli system ma 3 cyfry, jest niepoprawny (obsługujemy systemy od 2 do 16)
				if(c!=' ' && c!='\n'){
					while(c!=' ' && c!='\n'){
						czytajc;
					}
					fprintf(wyjscie,"\nError niepoprawna podstawa\n");
					err=1;
					continue;
				}	
			}
			//	Wczytujemy liczbę do zamiany
			while(c<'0'){
				czytajc
			}
			Vector l1;
			vector_init(&l1);
			czyt_liczbe(sys1,c,&l1);
			
			zmien_podstawe(&l1,sys1,sys2);
			if(err==1){
				continue;
			}
			wypisz(&l1);
			
		}else{
			// Inne działania (nie zmiana podstawy)
			int podstawa=0;
			char znak=c;
			czytajc
			
			czytajc
			
			podstawa=c-'0';
			
			czytajc
			if(c>='0' && c<='9'){
				podstawa*=10;
				podstawa+=c-'0';
				czytajc
			}
			while(c<'0'){
				if(c==EOF)break;
				czytajc
			}
			// Wczytujemy pierwszą liczbę - l1 i usuwamy zera wiodące
			ileEOL=0;
			Vector wynik;
			vector_init(&wynik);
			Vector l1;
			vector_init(&l1);
			czyt_liczbe(podstawa,c,&l1);
			if(err==1){
				continue;
			}
			usunzera(&l1);
			czytajc
			// Dopóki nie spotkamy trzech pustych linii, wczytujemy kolejne liczby do l2 i wykonujemy działanie zapisując wynik w l1
			while(ileEOL<3){
				if(c==EOF)break;
				while(c<'0' && ileEOL<3){
					if(c==EOF)break;
					if(c== '\n'){
						ileEOL++;
					}else{
						ileEOL=0;
					}
					czytajc
				}
				if(ileEOL<3){
					Vector l2;
					vector_init(&l2);
					vector_init(&wynik);
					czyt_liczbe(podstawa,c,&l2);
					usunzera(&l2);
					
					
					dzialanie(&l1,&l2,znak,podstawa,&wynik);
					if(err==1)break;
					vector_free(&l2);
					vector_copy(&l1,&wynik);
					vector_free(&wynik);
					
					czytajc
					if(c=='\n'){
						ileEOL=1;
					}else{
						ileEOL=0;
					}
					
					if(err==1){
						break;
					}
					
				}else{
					wypisz(&l1);
				}
			}
			// Jeżeli w trakcie wczytywania liczb i wykonywania działań skończył się plik i nie wypisaliśmy wyniku, wypisz
			if(ileEOL < 3 && c==EOF){
				wypisz(&l1);
			}
		}
	}
	fclose(wejscie);
	fclose(wyjscie);
}
