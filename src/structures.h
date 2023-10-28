#ifndef STRUCTURES_H
#define STRUCTURES_H

#define LONGUEUR_JUGEMENT 24
#define LONGUEUR_RESULTAT_JUGEMENT 25
#define LONGUEUR_CONDORCET 38
#define LONGUEUR_RESULTAT_CONDORCET 39
#define MAX_LINE_LENGTH 2048
#define MAX_VOTANTS 100
#define MAX_VOTES 100

typedef struct {
    int num_etu;
    char key[30];
} votant;

typedef struct {
    int reponse;
    char date[20];
    char cours[50];
    char hashed[64];
    int votes[10]; // votes [index burger]
} vote;


#endif // STRUCTURES_H
