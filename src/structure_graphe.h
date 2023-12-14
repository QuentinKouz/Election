#ifndef STRUCTURE_GRAPHE_H
#define STRUCTURES_GRAPHE_H

#include <stdbool.h>

#define MAX_CANDIDATS 10
#define MAX_COLONNES 20
#define MAX_LIGNES 100
#define MAX_TAILLE_CHAINE 100
#define MAX_LIGNE_TAILLE 1024
#define LIGNES 40
#define COLONNES 14

/************************************----------Structures de données----------*****************************************/
typedef struct {
    int indice;
    char nom[50];
} Candidat;

// Structure pour stocker la matrice de chaînes de caractères dynamique
typedef struct {
    char ***vote;  // Tableau de pointeurs vers des chaînes de caractères
    int ligne;     // Nombre de lignes
    int colonne;   // Nombre de colonnes
} t_mat_char_star_dyn;

typedef struct {
    int **donnees;
    int lignes;
    int colonnes;
} MatriceDuel;

typedef struct Arc {
    int sommetDepart;
    int sommetArrivee;
    int poids;
    struct Arc *suivant;
} Arc;

typedef struct ListeArcs{
    Arc *debut;
    unsigned taille;
    struct ListeArcs * suivant;
    int poids;
} ListeArcs;

typedef struct {
    ListeArcs * debut;
    ListeArcs * courant;    
    int taille;
} ListeDeListeArcs;

/*************************************************Méthodes**********************************************************/

const char* obtenirNomCandidat(int indice);

void lecture_csv_score_condorcet(const char *nom_fichier, t_mat_char_star_dyn *matrice, int ligne, int colonne);

void initMatrice(t_mat_char_star_dyn *matrice, int ligne, int colonne);

MatriceDuel creerMatriceDuel(int lignes, int colonnes);

void remplirMatriceDuel(const t_mat_char_star_dyn *matriceVote, MatriceDuel *matriceDuels);

ListeArcs creerListeArcs();

ListeDeListeArcs creerListeDeListeArcs();

bool estDansGraphe(ListeArcs* liste, int sommet);

void ajouterArc(ListeArcs *liste, const Arc *arc);

void retirerArc(ListeArcs* liste, Arc *arc);

void ajouterArcSommet(ListeArcs* graphe, ListeArcs* arcsAVerifier, int sommet);

void creerListeArcsDepuisMatrice(MatriceDuel matriceDuels, int nbCandidats, ListeArcs * liste);

void freeMatrice(t_mat_char_star_dyn *matrice);

void libererMatriceDuel(MatriceDuel *matrice);

void libererListeArcs(ListeArcs *liste);

void afficherMatriceDuelAvecNoms(const MatriceDuel *matrice, int nbCandidats);

void afficherListeArcs(const ListeArcs *liste);

void afficherListeDeListeArcs (ListeDeListeArcs * liste);

int weight_at(ListeDeListeArcs* liste, int ind);

#endif // STRUCTURE_GRAPHE_H