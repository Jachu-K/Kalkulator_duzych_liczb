#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int getc2(FILE * wejscie){
	int a=getc(wejscie);
	if(a=='\r'){
		a=getc(wejscie);
	}
	return a;
}
#define czytajc if(c==EOF)break;c=getc2(wejscie);if(c==EOF)break;putc(c,wyjscie);

FILE * wejscie;
FILE * wyjscie;

int ileEOL=0;
int err=0;

// wektor
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Vector;

// Funkcja inicjalizująca wektor
void vector_init(Vector *vec) {
	if(err)return;
	
    vec->size = 0;
    vec->capacity = 2;
    vec->data = (int *)malloc(vec->capacity * sizeof(int));
    if ((vec->data) == NULL) {
        fprintf(stderr, "ERROR 1 : brak miejsca w pamięci\n");
        err=1;
        return;
    }
}

// Funkcja zwalniająca pamięć zajmowaną przez wektor
void vector_free(Vector *vec) {	
	if(vec->data != NULL)
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

// Funkcja zmieniająca pojemność wektora
void vector_resize(Vector *vec, size_t new_capacity) {
	if(err)return;
	
	if(vec->data==NULL){
		vector_free(vec);
		vector_init(vec);
	}
	
	if(vec->size==0){
		vector_init(vec);
	}
	
    vec->data = (int *)realloc(vec->data, new_capacity * sizeof(int));
    if ((vec->data) == NULL) {
        fprintf(stderr, "ERROR 2 : brak miejsca w pamięci\n");
        err=1;
        return;
    }
    vec->capacity = new_capacity;
}

// Funkcja dodająca element na końcu wektora
void vector_push_back(Vector *vec, int watosc) {
	if(err)return;
	
    if (vec->size >= vec->capacity) {
        // Jeśli brak miejsca, zwiększ pojemność 2x
        vector_resize(vec, vec->capacity * 2);
    }
    vec->data[vec->size++] = watosc;
}

// Funkcja zwracająca rozmiar wektora
size_t vector_size(Vector *vec) {
    return vec->size;
}

// Funkcja odwracająca elementy wektora
void vector_reverse(Vector *vec) {
	if(err)return;
	
	if(vec->size==0)return;
    size_t left = 0;
    size_t right = vec->size - 1;
    
    while (left < right) {
        // Zamiana elementów: data[left] <--> data[right]
        int temp = vec->data[left];
        vec->data[left] = vec->data[right];
        vec->data[right] = temp;
        
        left++;
        right--;
    }
}

// Funkcja kopiująca wektor vec2 do vec1
void vector_copy(Vector *vec1, const Vector *vec2) {
	if(err)return;
	
    // Wyczyść vec1
    if (vec1->data != NULL) {
        free(vec1->data);
    }

    // Alokacja nowej pamięci
    vec1->data = (int *)malloc(vec2->capacity * sizeof(int));
    if (!vec1->data) {
        fprintf(stderr, "ERROR 5 : brak miejsca w pamięci\n");
        err=1;
        return;
    }

    // Kopiowanie danych z wektora 2 do wektora 1
    memcpy(vec1->data, vec2->data, vec2->size * sizeof(int));

    // Ustawienie rozmiaru i pojemności wektora 1 tak, by odpowiadały wektorowi 2
    vec1->size = vec2->size;
    vec1->capacity = vec2->capacity;
}

// Funkcja inicjująca wektor na odpowiedni rozmiar (i wypełniająca go zerami)
void ustawrozm(Vector *vec, size_t size) {
    vec->data = (int *)malloc(size * sizeof(int));
    if (!vec->data) {
        fprintf(stderr, "ERROR 9 : brak miejsca w pamięci\n");
        err=1;
        return;
    }
    vec->size = size;
    vec->capacity = size;
    memset(vec->data, 0, size * sizeof(int));  // Zapełnienie zerami
}

// Funkcja wycinająca fragment wektora
void vector_copy_range(Vector *dest, const Vector *src, size_t pocz, size_t kon) {
	if(err)return;
	
    ustawrozm(dest, kon - pocz);
    for (size_t i = pocz; i < kon; i++) {
        dest->data[i - pocz] = src->data[i];
    }
}

void wypisz(Vector *wektor){
	for (size_t i = 0; i < wektor->size; i++) {
		int wyp = wektor->data[i];
		if(wyp<10){
			putc(wyp+'0',wyjscie);
		}else{
			putc(wyp+'A'-10,wyjscie);
		}
	}
	putc('\n',wyjscie);
	fflush(wyjscie);
}

//dodawanie pisemne
void dodaj(Vector *a, Vector *b, int podstawa, Vector *wyn) {
	if(err)return;
	
	vector_free(wyn);
	vector_init(wyn);
    size_t poza = a->size;
    size_t pozb = b->size;

    if (poza == 0 || pozb == 0) {
        return;
    }

    int reszta = 0;
    while (poza > 0 || pozb > 0 || reszta > 0) {
        int suma = reszta;
        
        if (poza > 0) {
            poza--;
            suma += a->data[poza];
        }

        if (pozb > 0) {
            pozb--;
            suma += b->data[pozb];
        }

        reszta = suma / podstawa;
        suma %= podstawa;

        vector_push_back(wyn, suma);
    }

    vector_reverse(wyn);
}

// Funkcja rozszerzająca wektor dopisując 0 na początku
void rozszerz(Vector *vec, size_t new_size) {
	if(err)return;
	
    if (new_size <= vec->size) {
        return;
    }

    // Tworzymy nową tablicę
    int *new_data = (int *)malloc(new_size * sizeof(int));
    if (!new_data) {
        fprintf(wyjscie, "\nERROR 10 : Brak miejsca w pamięci\n");
        err=1;
        return;
    }

    // Wypełniamy początkowe miejsca zerami
    size_t ilezer = new_size - vec->size;
    memset(new_data, 0, ilezer * sizeof(int));

    // Kopiujemy stare dane do nowej tablicy
    memcpy(new_data + ilezer, vec->data, vec->size * sizeof(int));

    // Zwalniamy starą tablicę i ustawiamy nową
    if(vec->data != NULL)free(vec->data);
    vec->data = new_data;
    vec->size = new_size;
    vec->capacity = new_size;
}

//usun zera wiodace
void usunzera(Vector *wyn){
		
    size_t ilezer = 0;
    while (ilezer < wyn->size - 1 && wyn->data[ilezer] == 0) {
        ilezer++;
    }

    if (ilezer > 0) {
        memmove(wyn->data, wyn->data + ilezer, (wyn->size - ilezer) * sizeof(int));
        wyn->size -= ilezer;
    }
}


// Odejmowanie (odwracamy bity, dodajemy 1, odejmujemy odpowiednia potege)
void odejmij(Vector *vec1, Vector *vec2, Vector *wyn,int podstawa, int uszera) {
	if(err)return;
	
    Vector odwrocony_vec2;
    ustawrozm(&odwrocony_vec2, vec2->size);

    // Odwracamy cyfry vec2
    for (size_t i = 0; i < vec2->size; i++) {
        odwrocony_vec2.data[i] = podstawa-(vec2->data[i])-1;
    }

    Vector jeden;
    ustawrozm(&jeden, odwrocony_vec2.size);
    jeden.data[jeden.size - 1] = 1;

    Vector pocz_wynik;
    ustawrozm(&pocz_wynik, odwrocony_vec2.size + 1); 
    dodaj(&odwrocony_vec2, &jeden,podstawa, &pocz_wynik); // pocz_wynik = 1 + odw vec2

    dodaj(vec1, &pocz_wynik,podstawa, wyn); //wyn = 1 + odw vec2 + vec1

    // odejmujemy 1 w bicie starszym niż najstarszy bit odejmowanej liczby
    size_t gdzie_odjac = wyn->size - vec2->size - 1;
    if (gdzie_odjac < wyn->size) {

        if (wyn->data[gdzie_odjac] > 0) {
            wyn->data[gdzie_odjac] -= 1;
        } else {
            // Jeśli wartość na tej pozycji to 0 to pożyczamy
            int i = gdzie_odjac;
            while (i >= 0 && wyn->data[i] == 0) {
                wyn->data[i] = podstawa-1;
                i--;
            }
            if (i >= 0) {
                wyn->data[i]--;
            }
        }
    }

    if(uszera==1){
		usunzera(wyn);
	}
    vector_free(&odwrocony_vec2);
    vector_free(&jeden);
    vector_free(&pocz_wynik);
}

// Funkcja wyliczająca iloczyn dwóch liczb
void karacuba(Vector *vec1, Vector *vec2, Vector *wyn,int podstawa){
	if(err)return;
	
	size_t n = vec1->size > vec2->size ? vec1->size : vec2->size;
	rozszerz(vec1,n);
	rozszerz(vec2,n);
	if(n<=1){
		int product = vec1->data[0] * vec2->data[0];
		ustawrozm(wyn,2);
		wyn->data[1] = product%podstawa;
		wyn->data[0] = product/podstawa;
		if(wyn->data[0]==0){
			memmove(wyn->data, wyn->data+1,sizeof(int));
			wyn->size=1;
		}
		return;
	}
	
	n=(n+1)/2;
	
	Vector x1,x2,y1,y2,XX,YY,ZZ,temp1,temp2;
	
	//podzielenie vec1 i vec2 na dwie części
	vector_copy_range(&x1,vec1,0,vec1->size/2);
	vector_copy_range(&x2,vec1,vec1->size/2,vec1->size);
	vector_copy_range(&y1,vec2,0,vec2->size/2);
	vector_copy_range(&y2,vec2,vec2->size/2,vec2->size);
	
	karacuba(&x1,&y1,&XX,podstawa);
	karacuba(&x2,&y2,&YY,podstawa);
	
	vector_init(&temp1);
	vector_init(&temp2);
	
	dodaj(&x1,&x2,podstawa,&temp1);
	dodaj(&y1,&y2,podstawa,&temp2);

	karacuba(&temp1,&temp2, &ZZ,podstawa);

	Vector temp3,temp4;
	
	vector_init(&temp3);
	vector_init(&temp4);
		
	odejmij(&ZZ,&XX,&temp3,podstawa,1);
	vector_copy(&ZZ,&temp3);
	odejmij(&ZZ,&YY,&temp4,podstawa,1);
	vector_copy(&ZZ,&temp4);
	
	// Sklejenie wyniku
	
	vector_reverse(&XX);
	vector_reverse(&YY);
	vector_reverse(&ZZ);
	size_t rw =  XX.size > ZZ.size ? XX.size : ZZ.size;
    ustawrozm(wyn, rw + n * 2);
    
    for (size_t i = 0; i < YY.size; i++) wyn->data[i] += YY.data[i];
    for (size_t i = 0; i < ZZ.size; i++) wyn->data[i + n] += ZZ.data[i];
    for (size_t i = 0; i < XX.size; i++) wyn->data[i + n * 2] += XX.data[i];
    
    vector_reverse(wyn);
    
    //Przenoszenie reszt
    for(size_t i = 0; i < (wyn->size)-1; i++){
		while(wyn->data[wyn->size-i-1]>=podstawa){
			wyn->data[wyn->size-i-1]-=podstawa;
			wyn->data[wyn->size-i-2]++;
		}
	}
    
    
    // Usuwanie początkowych zer
    usunzera(wyn);
    
    vector_free(&x1);
    vector_free(&x2);
    vector_free(&y1);
    vector_free(&y2);
    vector_free(&ZZ);
    vector_free(&YY);
    vector_free(&XX);
    vector_free(&temp1);
    vector_free(&temp2);
    vector_free(&temp3);
    vector_free(&temp4);
}

// potęgowanie szybkie
void szybkie_potegowanie(Vector *a, Vector *b, int podstawa, Vector *wyn) {
    if(err) return;

    vector_init(wyn);
    vector_push_back(wyn, 1);

    Vector podpot, jeden;
    vector_init(&podpot);
    vector_init(&jeden);
    vector_copy(&jeden, wyn); 
    
    for (size_t i = 0; i < b->size; i++) {
		
        vector_copy(&podpot, &jeden);

        // podpot = a ^ cyfra(b)
        for (int j = 0; j < b->data[b->size - 1 - i]; j++) {
            Vector temp;
            vector_init(&temp);

            karacuba(&podpot, a, &temp, podstawa);

            vector_copy(&podpot, &temp);
            vector_free(&temp);
        }

        // wyn *= podpot
        Vector temp2;
        vector_init(&temp2);
        karacuba(&podpot, wyn, &temp2, podstawa);
        vector_copy(wyn, &temp2);
        vector_free(&temp2);

        // Domnażamy podpot do a^podstawa
        for (int j = podstawa; j > b->data[b->size - 1 - i]; j--) {
            Vector temp;
            vector_init(&temp);

            karacuba(&podpot, a, &temp, podstawa);

            vector_copy(&podpot, &temp);
            vector_free(&temp);
        }

        // a=podpot (= a^podstawa)
        vector_copy(a, &podpot);
    }

    vector_free(&podpot);
    vector_free(&jeden);
}


// porownanie wektora b przylozonego w odp miejscu z a
int porownaj(Vector *a, Vector *b, int przesb){
	if(err)return 0;
	
	for(size_t i=0; i < przesb; i++){
		if(a->data[i] != 0){
			return 1;
		}
	}
	for(size_t i=0; i < b->size; i++){
		if(a->data[i+przesb]<b->data[i]){
			return 0;
		}
		if(a->data[i+przesb]>b->data[i]){
			return 1;
		}
	}
	return 1;
}


// odjecie z przesunieciem
void odejmij_na_poz(Vector *a,Vector *b, size_t poz, int podstawa, Vector *wyn){
	if(err)return;
	
	size_t roza=a->size;
	size_t rozb=b->size;
	Vector temp;
	vector_init(&temp);
	vector_copy(&temp,b);
	for(size_t i=0; i < roza-rozb-poz; i++){
		vector_push_back(&temp,0);
	}
	Vector temp2;
	vector_init(&temp2);
	odejmij(a,&temp,&temp2,podstawa,0);
	vector_copy(wyn,&temp2);
	vector_free(&temp);
	vector_free(&temp2);
}

// dzielenie przez odejmowanie
void podziel(Vector *a,Vector *b, int podstawa, Vector *wyn, Vector *reszta){
	if(err)return;
	
	int niezero=0;
	for(size_t i=0;i<b->size;i++){
		if(b->data[i]!=0)niezero=1;
	}
	if(niezero==0){
		fprintf(wyjscie, "\nBłąd: dzielenie przez 0\n");
		err=1;
		return;
	}
	size_t roza = a->size;
	size_t rozb = b->size;
	if(rozb>roza){
		return;
	}
	int ile,x=0;
	for(size_t i = 0; i < roza-rozb+1; i++){
		ile=0;
		x = porownaj(a,b,i);
		// dopóki b przyłożone w danym miejscu jest mniejsze od a, odejmujemy
		while(x==1){
			Vector temp;
			vector_init(&temp);
			// a-b(na odp pozycji) = temp
			odejmij_na_poz(a,b,i,podstawa,&temp);
			vector_copy(a,&temp);
			if(a->size>roza){
				memmove(a->data, a->data + 1, (a->size - 1) * sizeof(int));
				a->size -= 1;
			}
			vector_free(&temp);
			
			ile++;
			x = porownaj(a,b,i);
		}
		// zapisujemy ile razy udało się odjąć
		vector_push_back(wyn,ile);
	}
	usunzera(wyn);
	vector_copy(reszta,a);
	usunzera(reszta);
	if(reszta->size==0)vector_push_back(reszta,0);
	if(wyn->size==0)vector_push_back(wyn,0);
}

//Funkcja decydująca o działaniu
void dzialanie(Vector *a,Vector *b,char symbol,int podstawa,Vector *wyn){
	Vector temp;
	vector_init(&temp);
	switch (symbol){
		case '+':
			dodaj(a,b,podstawa,wyn);
			break;
		case '*':
			karacuba(a,b,wyn,podstawa);
			break;
		case '/':
			podziel(a,b,podstawa,wyn,&temp);
			break;
		case '%':
			podziel(a,b,podstawa,&temp,wyn);
			break;
		case '^':
			szybkie_potegowanie(a,b,podstawa,wyn);
			break;
		default :
			fprintf(wyjscie, "\nERROR 6 : niepoprawny znak działania\n");
			err=1;
			return;
	}
	vector_free(&temp);
}

// porownanie liczb bez zer wiodacych
int porbezzer(Vector *a, Vector *b){
	if(a->size>b->size)return 1;
	if(a->size<b->size)return 0;
	for(size_t i=0;i<a->size;i++){
		if(a->data[i]>b->data[i])return 1;
		if(a->data[i]<b->data[i])return 0;
	}
	return 1;
}
	
// zmiana systemu liczbowego
void zmien_podstawe(Vector *vec1, int sys1, int sys2) {
	if(err)return;
	
    Vector potegi, pot0, wyn;
    vector_init(&potegi);
    vector_init(&pot0);
    vector_init(&wyn);

    vector_push_back(&potegi, 1);

    // Inicjalizacja `pot0` jako `sys2` reprezentowanego w systemie `sys1`
    int temp_sys2 = sys2;
    while (temp_sys2 > 0) {
        vector_push_back(&pot0, temp_sys2 % sys1);
        temp_sys2 /= sys1;
    }
    vector_reverse(&pot0); // Odwrócenie, by uzyskać poprawną reprezentację liczby

    int ilemnozen = 1;
    // Obliczamy największą potęgę `sys2` mniejszą niż `vec1` w systemie `sys1`
    while (porbezzer(vec1, &potegi)) {
        Vector temp2;
        vector_init(&temp2);

        karacuba(&potegi, &pot0, &temp2, sys1); // Mnożenie `potegi` przez `sys2` w `sys1`
        vector_copy(&potegi, &temp2);
        
        vector_free(&temp2);
        ilemnozen++;
    }

    // Pętla dzieląca `vec1` przez największe potęgi `sys2`
    for (int i = 0; i < ilemnozen; i++) {
		Vector temp10;
		vector_init(&temp10);
		int ile=0;
		while(porbezzer(vec1,&potegi)){
			odejmij(vec1,&potegi, &temp10 ,sys1,1);
			vector_copy(vec1,&temp10);
			ile++;
		}
		vector_push_back(&wyn,ile);

        // Zmniejszamy potęgę `potegi` dzieląc ją przez `pot0`
        Vector temp3, temp4;
        vector_init(&temp3);
        vector_init(&temp4);
        usunzera(&pot0);
        podziel(&potegi, &pot0, sys1, &temp3, &temp4);
        vector_copy(&potegi, &temp3);

        //przenoszenie reszt
        vector_reverse(&potegi);
        if(potegi.size>1){
			for(size_t j = 0; j < (potegi.size)-1; j++){
				while(potegi.data[j]>=sys1){
					potegi.data[j]-=sys1;
					potegi.data[j+1]++;
				}
			}
		}
		
		if(potegi.size==1){
			if(potegi.data[0]>=sys1){
				int t1=potegi.data[0] % sys1;
				int t2=potegi.data[0] / sys1;
				potegi.data[0]=t1;
				vector_push_back(&potegi,t2);
			}
		}
		vector_reverse(&potegi);

        vector_free(&temp3);
        vector_free(&temp4);
		
		usunzera(&wyn);
		usunzera(&potegi);
		usunzera(vec1);
    }

    vector_copy(vec1, &wyn);

    vector_free(&potegi);
    vector_free(&pot0);
    vector_free(&wyn);
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
