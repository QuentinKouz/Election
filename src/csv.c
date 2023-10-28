#include "structures.h"
#include "csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lecture_csv_votant(const char *nom_fichier, votant *votants[MAX_VOTANTS]) {
    FILE *file = fopen(nom_fichier, "r");

    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    int votants_count = 0;

    // on ignore la première ligne
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(file);
        exit(1);
    }

    int num_etu;
    char key[50];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d %s", &num_etu, &key) == 2) {
          votants[votants_count]->num_etu = num_etu;
          strcpy(votants[votants_count]->key, key);
          votants_count++;
        }
    }
    fclose(file);
}


void lecture_csv_vote(const char * nom_fichier, vote *votes[MAX_VOTES]) {
    FILE *file = fopen(nom_fichier, "r");

    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    int nb_votants = 0;

    // on ignore la première ligne
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(file);
        exit(1);
    }

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int field_count = 0;
        while (token != NULL && field_count < 14) {
            switch (field_count) {
                case 0:
                    votes[nb_votants]->reponse = atoi(token);
                    break;
                case 1:
                    strcpy(votes[nb_votants]->date, token);
                    break;
                case 2:
                    strcpy(votes[nb_votants]->cours, token);
                    break;
                case 3:
                    strcpy(votes[nb_votants]->hashed, token);
                    break;
                default:
                    votes[nb_votants]->votes[field_count-4] = atoi(token);
                    break;
            }
            token = strtok(NULL, ",");
            field_count++;
        }
        nb_votants++;
    }
    fclose(file);
}


void lecture_csv(char* nom_fichier, votant* votants[MAX_VOTANTS], vote* votes[MAX_VOTES]) {
    if (votes == NULL && votants != NULL) {
        lecture_csv_votant(nom_fichier, votants);
    } else if (votes != NULL && votants == NULL) {
        lecture_csv_vote(nom_fichier, votes);
    } else {
        fprintf(stderr, "erreur appel fonction\n");
        exit(1);
    }
}