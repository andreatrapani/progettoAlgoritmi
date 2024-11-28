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
    struct nodo *figli[];
    
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

//Metodo che inserisce il nodo tra i figli del padre
bool inserisciNodo(Nodo *nodoDaInserire, Nodo *nodoPadre){
    if(nodoDaInserire == NULL || nodoPadre == NULL) return false;
    nodoPadre->figli[nodoPadre->num_Figli] = nodoDaInserire;
    nodoPadre->num_Figli++;
    return true;
}

//Metodo che mi dice se dalla radice fino al fondo trova un nodo con più di 1 figlo
//se trova un nodo con più di un figlio bisogna effettuare uno stacco di un cavo
//Finito
bool isDaStaccare(Nodo *nodo){
    if(nodo == NULL) return false;
    if(nodo->num_Figli == 0) return false;
    if(nodo->num_Figli == 1) return isDaStaccare(nodo->figli[0]);
    if(nodo->num_Figli>1) return true;
}

//Metodo che trova quale è il nodo da staccare, che poi sposterò sotto un nodo foglia
//Lo stacca anche
//Finito
Nodo* trovaNodoDaStaccare(Nodo *nodo){
    int min = INT_MAX;
    Nodo *nodoDaStaccare;
    if(nodo == NULL) return NULL;
    if(nodo->num_Figli == 1) return trovaNodoDaStaccare(nodo->figli[0]);
    //Ciclo che trova qual'è il nodo con peso minore e lo salva in una variabile
    //che poi verrà restituita(sarà la parte di albero staccata)
    for(int x = 0; x < nodo->num_Figli; x++){
        if(nodo->figli[x]->value < min){
                nodoDaStaccare = nodo->figli[x];
                min = nodo->figli[x]->value;
        }
    }

    //ciclo che mette a null il figlio che abbiamo 
    //staccato(lo toglie dalla sua pos. originale)
    for(int x = 0; nodo->num_Figli;x++){
        if(nodo->figli[x] == nodoDaStaccare){
            nodo->figli[x] = NULL;
        }
    }
    return nodoDaStaccare;
}


//alla prima iterazione nodo è radice
//Metodo finito
void sposta(Nodo *nodoDaSpostare, Nodo *nodo){
    if(nodoDaSpostare == NULL || nodo == NULL) return;
    if(nodo->num_Figli == 0){
        nodo->figli[0] = nodoDaSpostare;
        nodo->num_Figli++;
        return;
    }
    sposta(nodoDaSpostare, nodo->figli[0]);
}









int main(){


    


    return 0;
}