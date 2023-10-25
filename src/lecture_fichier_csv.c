#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int num_etu;
    char key[50];
} votant;

#define MAX_VOTANTS 50

int lecture_csv(const char *fichier, votant vote[MAX_VOTANTS], int *nb_votants) {
    FILE *fichier_csv = fopen(fichier, "r");

    if (fichier_csv == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier CSV.\n");
        return 1;
    }

    *nb_votants = 0; // Initialise le compteur du nombre de votants lus
    char ligne[1024];

    // Lire et ignorer la première ligne (en-têtes/intitulés)
    if (fgets(ligne, sizeof(ligne), fichier_csv) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(fichier_csv);
        return 1;
    }

    while (fgets(ligne, sizeof(ligne), fichier_csv) && *nb_votants < MAX_VOTANTS) {

        int num_etu;
        char key[50];
        
        if (sscanf(ligne, "%d %s", &num_etu, key) == 2) {
            vote[*nb_votants].num_etu = num_etu;
            strncpy(vote[*nb_votants].key, key, sizeof(vote[*nb_votants].key));
            (*nb_votants)++;
        } 
        else {
            fprintf(stderr, "Erreur : La ligne ne contient pas deux champs séparés par des tabulations.\n");
        }
    }

    fclose(fichier_csv);

    return 0;
}

int main() {
    votant vote[MAX_VOTANTS]; // Tableau pour stocker les votants
    int nb_votants = 0;

    if (lecture_csv("codeJugementNumEtu.csv", vote, &nb_votants) == 0) {
        printf("Nombre de votants lus : %d\n", nb_votants);

        for (int i = 0; i < nb_votants; i++) {
            printf("Votant %d - Numéro d'étudiant : %d, Clé : %s\n", i + 1, vote[i].num_etu, vote[i].key);
        }
    }

    return 0;
}

