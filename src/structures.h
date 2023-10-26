#ifndef STRUCTURES_H
#define STRUCTURES_H

#define LONGUEUR_JUGEMENT 24
#define LONGUEUR_RESULTAT_JUGEMENT 25
#define LONGUEUR_CONDORCET 38
#define LONGUEUR_RESULTAT_CONDORCET 39

typedef struct {
    int num_etu;
    char key[50];
} votant;

typedef struct {
    int reponse;
    char* date;
    char* cours;
    char* hashed;
    int * votes[]; // votes [index burger] [notes burger]
} vote;


#endif // STRUCTURES_H