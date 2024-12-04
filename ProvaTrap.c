#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

// Definizione della struttura del nodo
typedef struct nodo {
    int value;            // Valore del nodo
    int indice;           // Indice del nodo
    int num_Figli;        // Numero di figli attuali
    int max_Figli;        // Numero massimo di figli
    struct nodo **figli;  // Puntatori ai figli
} Nodo;

// Funzione ricorsiva per stampare i nodi dell'albero
void stampaNodi(Nodo* nodo) {
    if (nodo == NULL) return;

    printf("nodo[%d] -> (%d) -> numF(%d) -> {", nodo->indice, nodo->value, nodo->num_Figli);
    for (int i = 0; i < nodo->num_Figli; i++) {
        printf("nodo[%d]", nodo->figli[i]->indice);
        if (i < nodo->num_Figli - 1) printf(", ");
    }
    printf("}\n");

    for (int i = 0; i < nodo->num_Figli; i++) {
        stampaNodi(nodo->figli[i]);
    }
}

// Funzione che cerca un nodo dato un indice
Nodo* trovaNodo(Nodo* nodo, int indice) {
    if (nodo == NULL) return NULL;
    if (nodo->indice == indice) return nodo;

    for (int x = 0; x < nodo->num_Figli; x++) {
        Nodo *risultato = trovaNodo(nodo->figli[x], indice);
        if (risultato != NULL) return risultato;
    }
    return NULL;
}

// Funzione per inserire un nodo come figlio
bool inserisciNodo(Nodo *nodoDaInserire, Nodo *nodoPadre) {
    if (nodoDaInserire == NULL || nodoPadre == NULL) return false;

    if (nodoPadre->num_Figli >= nodoPadre->max_Figli) {
        fprintf(stderr, "Errore: superato il limite di figli per il nodo %d.\n", nodoPadre->indice);
        return false;
    }

    nodoPadre->figli[nodoPadre->num_Figli] = nodoDaInserire;
    nodoPadre->num_Figli++;
    return true;
}

// Funzione per creare un nuovo nodo
Nodo* creaNodo(int valore, int indice, int max_Figli) {
    Nodo *nodo = (Nodo *)malloc(sizeof(Nodo));
    if (nodo == NULL) {
        fprintf(stderr, "Errore: impossibile allocare memoria per il nodo.\n");
        return NULL;
    }

    nodo->value = valore;
    nodo->indice = indice;
    nodo->num_Figli = 0;
    nodo->max_Figli = max_Figli;

    if (max_Figli > 0) {
        nodo->figli = (Nodo **)malloc(max_Figli * sizeof(Nodo *));
        if (nodo->figli == NULL) {
            fprintf(stderr, "Errore: impossibile allocare memoria per i figli.\n");
            free(nodo);
            return NULL;
        }
    } else {
        nodo->figli = NULL;
    }

    return nodo;
}

// Funzione per calcolare il numero di figli dato l'indice
int trovaFigli(const int connessioni[], int numNodi, int indice) {
    int numFigli = 0;
    for (int i = 0; i < numNodi; i++) {
        if (connessioni[i] == indice) numFigli++;
    }
    return numFigli;
}

// Funzione per creare l'albero
Nodo* creaAlbero(FILE *file, int *numNodi) {
    if (fscanf(file, "%d", numNodi) != 1) {
        fprintf(stderr, "Errore nella lettura del numero di nodi.\n");
        return NULL;
    }

    int connessioni[*numNodi];
    int valori[*numNodi];

    for (int i = 0; i < *numNodi; i++) {
        if (fscanf(file, "%d", &connessioni[i]) != 1) {
            fprintf(stderr, "Errore nella lettura delle connessioni.\n");
            return NULL;
        }
    }

    for (int i = 0; i < *numNodi; i++) {
        if (fscanf(file, "%d", &valori[i]) != 1) {
            fprintf(stderr, "Errore nella lettura dei valori.\n");
            return NULL;
        }
    }

    Nodo *radice = creaNodo(0, 0, (trovaFigli(connessioni, *numNodi, 0)+1));
    if (radice == NULL) return NULL;

    for (int i = 0; i < *numNodi; i++) {
        int numFigli = trovaFigli(connessioni, *numNodi, i+1);
        Nodo *nodoInterno = creaNodo(valori[i], i+1, (numFigli +1));

        if (nodoInterno == NULL) return NULL;

        Nodo *nodoPadre = trovaNodo(radice, connessioni[i]);
        if (nodoPadre == NULL || !inserisciNodo(nodoInterno, nodoPadre)) {
            fprintf(stderr, "Errore nell'inserimento del nodo %d nel padre %d.\n", nodoInterno->indice, connessioni[i]);
            return NULL;
        }
    }

    return radice;
}

// Funzione ricorsiva per liberare l'albero
void liberaAlbero(Nodo *nodo) {
    if (nodo == NULL) return;

    // Libera i figli ricorsivamente
    for (int i = 0; i < nodo->num_Figli; i++) {
        liberaAlbero(nodo->figli[i]);
    }

    // Libera l'array dei figli
    free(nodo->figli);

    // Libera il nodo stesso
    free(nodo);
}

//Funzione booleana per trovare un nodo da staccare a partire dalla radice
bool isDaStaccare(Nodo *nodo) {
    if (nodo == NULL) return false;  // Base case: nodo nullo
    if (nodo->num_Figli > 1) return true;  // Nodo con più di un figlio
    if (nodo->num_Figli == 1) return isDaStaccare(nodo->figli[0]);  // Ricorsione per l'unico figlio
    return false;  // Nodo senza figli
}

//Funzione che trova quale è il nodo da staccare, che poi sposterò sotto un nodo foglia
Nodo* trovaNodoDaStaccare(Nodo *nodo) {
    if (nodo == NULL || nodo->num_Figli == 0) return NULL;

    int min = INT_MAX;
    int indexNodo = -1; // Inizializza l'indice come non valido
    Nodo *nodoDaStaccare = NULL;

    // Cerca il figlio con valore minimo
    for (int i = 0; i < nodo->num_Figli; i++) {
        if (nodo->figli[i]->value < min) {
            min = nodo->figli[i]->value;
            nodoDaStaccare = nodo->figli[i];
            indexNodo = i;
        }
    }

    // Se trovi un nodo da staccare, rimuovilo dai figli
    if (indexNodo != -1) {
        // Shift degli altri figli per riempire il "buco"
        for (int i = indexNodo; i < nodo->num_Figli - 1; i++) {
            nodo->figli[i] = nodo->figli[i + 1];
        }
        nodo->figli[nodo->num_Figli - 1] = NULL; // Pulizia
        nodo->num_Figli--; // Aggiorna il conteggio dei figli
    }

    return nodoDaStaccare;
}

void sposta(Nodo *nodoDaSpostare, Nodo *nodo){
    if(nodoDaSpostare == NULL || nodo == NULL) return;
    if(nodo->num_Figli == 0){
        nodo->figli[0] = nodoDaSpostare;
        nodo->num_Figli++;
        return;
    }
    sposta(nodoDaSpostare, nodo->figli[0]);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nome_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }

    int numNodi = 0;
    Nodo *radice = creaAlbero(file, &numNodi);
    fclose(file);

    if (radice != NULL) {
        stampaNodi(radice);
        int peso = 0;
        Nodo *nodoPadre = radice;

        while (isDaStaccare(nodoPadre)) {
            Nodo* nodoDaStaccare = trovaNodoDaStaccare(nodoPadre);
            if (nodoDaStaccare != NULL) {
                sposta(nodoDaStaccare, radice);
                peso += nodoDaStaccare->value;
            }
        }

        printf("\n\n --- NODI SPOSTATI --- \n\n");
        stampaNodi(radice);
        printf("\n\nCOSTO TOTALE = %d\n", peso);
        liberaAlbero(radice);
    }

    return 0;
}
