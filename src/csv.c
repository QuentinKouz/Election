#include "structures.h"
#include "csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_LENGTH 1024
#define MAX_VOTANTS 100
#define MAX_VOTES 100


void lecture_csv_votant(const char * nom_fichier, votant *votants[MAX_VOTANTS]) {
    FILE *file = fopen(nom_fichier, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }
    printf("fichier %s ouvert avec succès\n", nom_fichier);

    char line[MAX_LINE_LENGTH];
    int votants_count = 0;

    // on ignore la première ligne
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(file);
        exit(1);
    }

        printf("Début lecture fichier\n");
    int num_etu;
    char key[50];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d %s", &votants[votants_count]-> num_etu, &votants[votants_count]-> key) == 2) {
            printf("num_etu: %d key: %s\n", votants[votants_count]->num_etu, votants[votants_count]->key);
            votants_count++;
        }
    }
    fclose(file);
}


void lecture_csv_vote(const char * nom_fichier, vote votes[MAX_VOTES]) {
    FILE *file = fopen(nom_fichier, "r");


    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }


    char line[MAX_LINE_LENGTH];
    int nb_votants = 0;


    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int field_count = 0;
        while (token != NULL && field_count < 14) {
            switch (field_count) {
                case 0:
                    votes[nb_votants].reponse = atoi(token);
                    break;
                case 1:
                    strcpy(votes[nb_votants].date, token);
                    break;
                case 2:
                    strcpy(votes[nb_votants].cours, token);
                    break;
                case 3:
                    strcpy(votes[nb_votants].hashed, token);
                    break;
                default:
                    votes[nb_votants].votes[field_count-4][0] = atoi(token);
                    break;
            }
            token = strtok(NULL, ",");
            field_count++;
        }
        nb_votants++;
    }


    fclose(file);
}


void lecture_csv(char* nom_fichier, votant* votants, vote* votes) {
    if (votes == NULL && votants != NULL) {
        lecture_csv_votant(nom_fichier, &votants);
    } else if (votes != NULL && votants == NULL) {
        lecture_csv_vote(nom_fichier, &votes);
    } else {
        fprintf(stderr, "erreur appel fonction\n");
        exit(1);
    }
}



/*

#define MAX_VOTES 50
#define MAX_VOTANTS 50


int lecture_csv_votes(const char *fichier, vote votes[MAX_VOTES], int *nb_votes) {
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

int lecture_csv_votant(const char *fichier, votant vote[MAX_VOTANTS], int *nb_votants) {
    FILE *fichier_csv = fopen(fichier, "r");

    if (fichier_csv == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier CSV.\n");
        return 1;
    }
    printf("TEST 1\n");

    *nb_votants = 0; // Initialise le compteur du nombre de votants lus
    char ligne[1024];

    // Lire et ignorer la première ligne (en-têtes/intitulés)
    if (fgets(ligne, sizeof(ligne), fichier_csv) == NULL) {
        fprintf(stderr, "Le fichier est vide.\n");
        fclose(fichier_csv);
        return 1;
    }
    printf("TEST 2\n");

    while (fgets(ligne, sizeof(ligne), fichier_csv) && *nb_votants < MAX_VOTANTS) {
      printf("TEST 3\n");

        int num_etu;
        char key[50];

        if (sscanf(ligne, "%d %s", &num_etu, key) == 2) {
            printf("TEST 4\n");

            vote[*nb_votants].num_etu = num_etu;

            strncpy(vote[*nb_votants].key, key, sizeof(vote[*nb_votants].key+1));

            printf("num: %d key: %s\n", vote[*nb_votants].num_etu, vote[*nb_votants].key);
            (*nb_votants)++;
        }
        else {
            fprintf(stderr, "Erreur : La ligne ne contient pas deux champs séparés par des tabulations.\n");
        }
    }
    printf("TEST 5\n");

    fclose(fichier_csv);

    return 0;
}


void lecture_csv(char* fichier, votant *votants, vote *votes){
   const char *chaine_votes = "./ResultatsVote/vote";
   const char *chaine_votants = "./ResultatsVote/code";
   const char *chaine_jugement = "Jugement";
   const char *chaine_condorcet = "Condorcet";
   const char *file = fichier;
   int nb_votant;
   if (strncmp(fichier, chaine_votes, strlen(chaine_votes)) == 0){
     if (strstr(file, chaine_jugement) != NULL){
       lecture_csv_votes(fichier,&votes, LONGUEUR_RESULTAT_JUGEMENT);
     } else if (strstr(file, chaine_condorcet) != NULL){
       lecture_csv_votes(fichier,&votes, LONGUEUR_RESULTAT_CONDORCET);
     } else {
       printf("Erreur fichier votes\n");
     }
   } else if((strncmp(file, chaine_votants, strlen(chaine_votants)) == 0)){
     if (strstr(file, chaine_jugement) != NULL){
       printf("ici\n");
       lecture_csv_votant(fichier,&votants, &nb_votant);
     } else if (strstr(file, chaine_condorcet) != NULL){
       lecture_csv_votant(fichier,&votants, LONGUEUR_CONDORCET);
     } else {
       printf("Erreur fichier votants\n");
     }
   } else {
     printf("Aucun fichier n'a pu être identifié au fichier demandé.\n");
     exit(1);
   }
}
*/