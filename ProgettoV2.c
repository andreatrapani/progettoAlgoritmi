#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#define MAXFIGLI 100


typedef struct nodo{
    int value;
    int indice;
    int num_Figli;
    struct nodo *figli[MAXFIGLI];
    
}Nodo;

//Metodo che dato un indice parte dalla radice e trova il nodo.
Nodo* trovaNodo(Nodo* nodo, int indice){
    if(nodo == NULL) return NULL;

    //Controllo se il nodo corrente abbia l'indice cercato
    if(nodo->indice == indice) return nodo;

    //Cerco ricorsivamente nei figli
    for(int x = 0; x < nodo->num_Figli; x++){
        Nodo *risultato = trovaNodo(nodo->figli[x], indice);
        if(risultato != NULL){
            //Nodo trovato
            return risultato;
        }
    }
    //Nodo non trovato
    return NULL;
}

bool inserisciNodo(Nodo *nodoDaInserire, Nodo *nodoPadre){
    if(nodoDaInserire == NULL || nodoPadre == NULL) return false;
    nodoPadre->figli[nodoPadre->num_Figli] = nodoDaInserire;
    nodoPadre->num_Figli++;
    return true;
}

bool isDaStaccare(Nodo *nodo){
    if(nodo == NULL) return false;
    if(nodo->num_Figli == 0) return false;
    if(nodo->num_Figli == 1) return isDaStaccare(nodo->figli[0]);
    if(nodo->num_Figli>1) return true;
}

Nodo* trovaNodoDaStaccare(Nodo *nodo){
    int min = INT_MAX;
    Nodo *nodoDaStaccare;
    if(nodo == NULL) return NULL;
    if(nodo->num_Figli == 1) return trovaNodoDaStaccare(nodo->figli[0]);
    for(int x = 0; x < nodo->num_Figli; x++){
        if(nodo->figli[x]->value < min){
                nodoDaStaccare = nodo->figli[x];
        }
    }
    return nodoDaStaccare;
}



void sposta(Nodo *nodoDaStaccare, Nodo *nodo){
    if(nodoDaStaccare == NULL || nodo == NULL) return;
    while(nodo->num_Figli != 0){
        if(nodo->figli[0] == nodoDaStaccare){
            nodo = nodo->figli[1];
        } else{
            nodo = nodo->figli[0];
        }
    }
    nodo->figli[0] = nodoDaStaccare;
}









int main(){


    


    return 0;
}