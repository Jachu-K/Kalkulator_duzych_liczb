#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int err;
extern FILE *wyjscie;

void dodaj(Vector *a, Vector *b, int podstawa, Vector *wyn) {
    if(err) return;
    
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

void odejmij(Vector *vec1, Vector *vec2, Vector *wyn, int podstawa, int uszera) {
    if(err) return;
    
    Vector odwrocony_vec2;
    ustawrozm(&odwrocony_vec2, vec2->size);

    for (size_t i = 0; i < vec2->size; i++) {
        odwrocony_vec2.data[i] = podstawa - (vec2->data[i]) - 1;
    }

    Vector jeden;
    ustawrozm(&jeden, odwrocony_vec2.size);
    jeden.data[jeden.size - 1] = 1;

    Vector pocz_wynik;
    ustawrozm(&pocz_wynik, odwrocony_vec2.size + 1); 
    dodaj(&odwrocony_vec2, &jeden, podstawa, &pocz_wynik);

    dodaj(vec1, &pocz_wynik, podstawa, wyn);

    size_t gdzie_odjac = wyn->size - vec2->size - 1;
    if (gdzie_odjac < wyn->size) {
        if (wyn->data[gdzie_odjac] > 0) {
            wyn->data[gdzie_odjac] -= 1;
        } else {
            int i = gdzie_odjac;
            while (i >= 0 && wyn->data[i] == 0) {
                wyn->data[i] = podstawa - 1;
                i--;
            }
            if (i >= 0) {
                wyn->data[i]--;
            }
        }
    }

    if(uszera == 1){
        usunzera(wyn);
    }
    vector_free(&odwrocony_vec2);
    vector_free(&jeden);
    vector_free(&pocz_wynik);
}

void karacuba(Vector *vec1, Vector *vec2, Vector *wyn, int podstawa) {
    if(err) return;
    
    size_t n = vec1->size > vec2->size ? vec1->size : vec2->size;
    rozszerz(vec1, n);
    rozszerz(vec2, n);
    
    if(n <= 1){
        int product = vec1->data[0] * vec2->data[0];
        ustawrozm(wyn, 2);
        wyn->data[1] = product % podstawa;
        wyn->data[0] = product / podstawa;
        if(wyn->data[0] == 0){
            memmove(wyn->data, wyn->data + 1, sizeof(int));
            wyn->size = 1;
        }
        return;
    }
    
    n = (n + 1) / 2;
    
    Vector x1, x2, y1, y2, XX, YY, ZZ, temp1, temp2;
    
    vector_copy_range(&x1, vec1, 0, vec1->size / 2);
    vector_copy_range(&x2, vec1, vec1->size / 2, vec1->size);
    vector_copy_range(&y1, vec2, 0, vec2->size / 2);
    vector_copy_range(&y2, vec2, vec2->size / 2, vec2->size);
    
    karacuba(&x1, &y1, &XX, podstawa);
    karacuba(&x2, &y2, &YY, podstawa);
    
    vector_init(&temp1);
    vector_init(&temp2);
    
    dodaj(&x1, &x2, podstawa, &temp1);
    dodaj(&y1, &y2, podstawa, &temp2);

    karacuba(&temp1, &temp2, &ZZ, podstawa);

    Vector temp3, temp4;
    
    vector_init(&temp3);
    vector_init(&temp4);
        
    odejmij(&ZZ, &XX, &temp3, podstawa, 1);
    vector_copy(&ZZ, &temp3);
    odejmij(&ZZ, &YY, &temp4, podstawa, 1);
    vector_copy(&ZZ, &temp4);
    
    vector_reverse(&XX);
    vector_reverse(&YY);
    vector_reverse(&ZZ);
    
    size_t rw = XX.size > ZZ.size ? XX.size : ZZ.size;
    ustawrozm(wyn, rw + n * 2);
    
    for (size_t i = 0; i < YY.size; i++) wyn->data[i] += YY.data[i];
    for (size_t i = 0; i < ZZ.size; i++) wyn->data[i + n] += ZZ.data[i];
    for (size_t i = 0; i < XX.size; i++) wyn->data[i + n * 2] += XX.data[i];
    
    vector_reverse(wyn);
    
    for(size_t i = 0; i < (wyn->size) - 1; i++){
        while(wyn->data[wyn->size - i - 1] >= podstawa){
            wyn->data[wyn->size - i - 1] -= podstawa;
            wyn->data[wyn->size - i - 2]++;
        }
    }
    
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

        for (int j = 0; j < b->data[b->size - 1 - i]; j++) {
            Vector temp;
            vector_init(&temp);
            karacuba(&podpot, a, &temp, podstawa);
            vector_copy(&podpot, &temp);
            vector_free(&temp);
        }

        Vector temp2;
        vector_init(&temp2);
        karacuba(&podpot, wyn, &temp2, podstawa);
        vector_copy(wyn, &temp2);
        vector_free(&temp2);

        for (int j = podstawa; j > b->data[b->size - 1 - i]; j--) {
            Vector temp;
            vector_init(&temp);
            karacuba(&podpot, a, &temp, podstawa);
            vector_copy(&podpot, &temp);
            vector_free(&temp);
        }

        vector_copy(a, &podpot);
    }

    vector_free(&podpot);
    vector_free(&jeden);
}

int porownaj(Vector *a, Vector *b, size_t przesb) {
    if(err) return 0;
    
    for(size_t i = 0; i < przesb; i++){
        if(a->data[i] != 0){
            return 1;
        }
    }
    
    for(size_t i = 0; i < b->size; i++){
        if(a->data[i + przesb] < b->data[i]){
            return 0;
        }
        if(a->data[i + przesb] > b->data[i]){
            return 1;
        }
    }
    return 1;
}

void odejmij_na_poz(Vector *a, Vector *b, size_t poz, int podstawa, Vector *wyn) {
    if(err) return;
    
    size_t roza = a->size;
    size_t rozb = b->size;
    Vector temp;
    vector_init(&temp);
    vector_copy(&temp, b);
    
    for(size_t i = 0; i < roza - rozb - poz; i++){
        vector_push_back(&temp, 0);
    }
    
    Vector temp2;
    vector_init(&temp2);
    odejmij(a, &temp, &temp2, podstawa, 0);
    vector_copy(wyn, &temp2);
    
    vector_free(&temp);
    vector_free(&temp2);
}

void podziel(Vector *a, Vector *b, int podstawa, Vector *wyn, Vector *reszta) {
    if(err) return;
    
    int niezero = 0;
    for(size_t i = 0; i < b->size; i++){
        if(b->data[i] != 0) niezero = 1;
    }
    
    if(niezero == 0){
        fprintf(wyjscie, "\nBłąd: dzielenie przez 0\n");
        err = 1;
        return;
    }
    
    size_t roza = a->size;
    size_t rozb = b->size;
    
    if(rozb > roza){
        return;
    }
    
    int ile, x = 0;
    for(size_t i = 0; i < roza - rozb + 1; i++){
        ile = 0;
        x = porownaj(a, b, i);
        
        while(x == 1){
            Vector temp;
            vector_init(&temp);
            odejmij_na_poz(a, b, i, podstawa, &temp);
            vector_copy(a, &temp);
            
            if(a->size > roza){
                memmove(a->data, a->data + 1, (a->size - 1) * sizeof(int));
                a->size -= 1;
            }
            
            vector_free(&temp);
            ile++;
            x = porownaj(a, b, i);
        }
        
        vector_push_back(wyn, ile);
    }
    
    usunzera(wyn);
    vector_copy(reszta, a);
    usunzera(reszta);
    
    if(reszta->size == 0) vector_push_back(reszta, 0);
    if(wyn->size == 0) vector_push_back(wyn, 0);
}

void dzialanie(Vector *a, Vector *b, char symbol, int podstawa, Vector *wyn) {
    Vector temp;
    vector_init(&temp);
    switch (symbol){
        case '+':
            dodaj(a, b, podstawa, wyn);
            break;
        case '*':
            karacuba(a, b, wyn, podstawa);
            break;
        case '/':
            podziel(a, b, podstawa, wyn, &temp);
            break;
        case '%':
            podziel(a, b, podstawa, &temp, wyn);
            break;
        case '^':
            szybkie_potegowanie(a, b, podstawa, wyn);
            break;
        default:
            fprintf(wyjscie, "\nERROR 6 : niepoprawny znak działania\n");
            err = 1;
            return;
    }
    
    //vector_free(&temp);
}

void zmien_podstawe(Vector *vec1, int sys1, int sys2) {
    if(err) return;
    
    Vector potegi, pot0, wyn;
    vector_init(&potegi);
    vector_init(&pot0);
    vector_init(&wyn);

    vector_push_back(&potegi, 1);

    int temp_sys2 = sys2;
    while (temp_sys2 > 0) {
        vector_push_back(&pot0, temp_sys2 % sys1);
        temp_sys2 /= sys1;
    }
    vector_reverse(&pot0);

    int ilemnozen = 1;
    while (porbezzer(vec1, &potegi)) {
        Vector temp2;
        vector_init(&temp2);
        karacuba(&potegi, &pot0, &temp2, sys1);
        vector_copy(&potegi, &temp2);
        vector_free(&temp2);
        ilemnozen++;
    }

    for (int i = 0; i < ilemnozen; i++) {
        Vector temp10;
        vector_init(&temp10);
        int ile = 0;
        
        while(porbezzer(vec1, &potegi)) {
            odejmij(vec1, &potegi, &temp10, sys1, 1);
            vector_copy(vec1, &temp10);
            ile++;
        }
        
        vector_push_back(&wyn, ile);

        Vector temp3, temp4;
        vector_init(&temp3);
        vector_init(&temp4);
        usunzera(&pot0);
        podziel(&potegi, &pot0, sys1, &temp3, &temp4);
        vector_copy(&potegi, &temp3);

        vector_reverse(&potegi);
        if(potegi.size > 1){
            for(size_t j = 0; j < (potegi.size) - 1; j++){
                while(potegi.data[j] >= sys1){
                    potegi.data[j] -= sys1;
                    potegi.data[j + 1]++;
                }
            }
        }
        
        if(potegi.size == 1){
            if(potegi.data[0] >= sys1){
                int t1 = potegi.data[0] % sys1;
                int t2 = potegi.data[0] / sys1;
                potegi.data[0] = t1;
                vector_push_back(&potegi, t2);
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
