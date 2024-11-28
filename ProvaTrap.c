#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#define MAXFIGLI 100

typedef struct nodo {
    int value;              // Valore del nodo
    int indice;             // Indice del nodo
    int num_Figli;          // Numero di figli
    struct nodo *figli[MAXFIGLI]; // Puntatori ai figli
} Nodo;

// Funzione per creare un albero
Nodo* creaAlbero(FILE *file, int *numNodi) {
    char buffer[256];  // Buffer per memorizzare la prima riga

    // Leggi la prima riga dal file
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Estrai il primo numero intero dalla riga
        if (sscanf(buffer, "%d", numNodi) != 1) {
            fprintf(stderr, "Errore: impossibile leggere un numero dalla prima riga.\n");
            return NULL; // Restituisce errore se non trova un numero
        }
    } else {
        fprintf(stderr, "Errore: il file è vuoto o non leggibile.\n");
        return NULL; // Restituisce errore se fgets fallisce
    }

    // Alloca memoria per il nodo radice
    Nodo* radice = (Nodo*)malloc(sizeof(Nodo));
    if (radice == NULL) {
        fprintf(stderr, "Errore: impossibile allocare memoria per il nodo radice.\n");
        return NULL;
    }

    // Inizializza il nodo radice
    radice->value = 0;         // Puoi cambiare il valore iniziale
    radice->indice = 0;        // Indice della radice
    radice->num_Figli = trovaFigli(/*passo la seconda riga come stringa*/, radice->indice);
 
    return radice;
}

int trovaFigli(char[] figli, int indice){

}

int main(int argc, char *argv[]) {
    FILE *file;
    int numNodi = 0;

    if (argc != 2) {  // Controlla che venga passato un solo argomento
        fprintf(stderr, "Uso: %s <nome_file>\n", argv[0]);
        return 1; // Termina il programma con errore
    }
    
    // Apri il file specificato come argomento
    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return 1; // Termina il programma con errore
    }

    // Crea l'albero
    Nodo* radice = creaAlbero(file, &numNodi);
    if (radice != NULL) {
        printf("Numero di nodi letto dalla prima riga: %d\n", numNodi);
        // Qui puoi lavorare con l'albero
        free(radice); // Libera la memoria del nodo radice
    }

    // Chiudi il file
    fclose(file);
    return 0;
}
