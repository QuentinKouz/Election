#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int reponse;
    char* date;
    char* cours;
    char* hashed;
    int * votes[10]; // Tableau de votes pour 10 burgers
} vote;

#define MAX_VOTES 50

// Libère la mémoire allouée pour un vote
void liberer_vote(vote* v) {
    free(v->date);
    free(v->cours);
    free(v->hashed);
    for (int i = 0; i < 10; i++) {
        free(v->votes[i]);
    }
}

int lecture_csv2(const char *fichier, vote votes[MAX_VOTES], int *nb_votes) {
    FILE *fichier_csv = fopen(fichier, "r");

    if (fichier_csv == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier CSV.\n");
        return 1;
    }
    *nb_votes = 0;
    char line[1024];

    if (fgets(line, sizeof(line), fichier_csv) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(fichier_csv);
        return 1;
    }

    while (fgets(line, sizeof(line), fichier_csv) && *nb_votes < MAX_VOTES) {

        char *data = strtok(line, ",");
        if (data != NULL) {
            votes[*nb_votes].reponse = atoi(data);
        }

        data = strtok(NULL, ",");
        if (data != NULL) {
            votes[*nb_votes].date = strdup(data);
        }

        data = strtok(NULL, ",");
        if (data != NULL) {
            votes[*nb_votes].cours = strdup(data);
        }

        data = strtok(NULL, ",");
        if (data != NULL) {
            votes[*nb_votes].hashed = strdup(data);
        }

        for (int i = 0; i < 10; i++) {
            data = strtok(NULL, ",");
            if (data != NULL) {
                // Allouer de la mémoire pour le tableau de votes
                votes[*nb_votes].votes[i] = (int *)malloc(sizeof(int));
                *(votes[*nb_votes].votes[i]) = atoi(data);
            }
        }
        (*nb_votes)++;
    }
    fclose(fichier_csv);
    return 0;
}

int main() {
    vote votes[MAX_VOTES];
    int nb_votes = 0;

    if (lecture_csv2("VoteCondorcet.csv", votes, &nb_votes) == 0) {
        printf("Nombre de votes lus : %d\n", nb_votes);

        for (int i = 0; i < nb_votes; i++) {
            printf("Vote %d - Réponse : %d, Date : %s, Cours : %s, Hash : %s\n", i + 1, votes[i].reponse, votes[i].date, votes[i].cours, votes[i].hashed);
            printf("Votes pour chaque burger : ");

            for (int j = 0; j < 10; j++) {
                printf("%d ", *(votes[i].votes[j]));
            }

            printf("\n");

            // Libérer la mémoire allouée pour ce vote
            liberer_vote(&votes[i]);
        }
    }

    return 0;
}
