#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
    Arc *debut;
} ListeArcs;

/************************************----------CANDIDATS----------*****************************************/

Candidat candidats[MAX_CANDIDATS] = {
    {0, "BBP"}, {1, "BSO"}, {2, "Thai"}, {3, "Veggie"}, {4, "FireC"},
    {5, "Roma"}, {6, "Crispy"}, {7, "Cheese"}, {8, "BS"}, {9, "Country"},
};

// Fonction pour obtenir le nom du candidat à partir de son indice
const char* obtenirNomCandidat(int indice) {
    for (int i = 0; i < MAX_CANDIDATS; ++i) {
        if (candidats[i].indice == indice) {
            return candidats[i].nom;
        }
    }
    return NULL; // Indice non trouvé
}


/************************************----------LECTURE CSV----------*****************************************/

void lecture_csv(const char *nom_fichier, t_mat_char_star_dyn *matrice, int ligne, int colonne) {
    FILE *fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_LIGNE_TAILLE];

    // Allouer mémoire pour les pointeurs de lignes
    matrice->vote = (char ***)malloc(ligne * sizeof(char **));

    // Lire les données du fichier et remplir la matrice
    for (int i = 0; i < ligne && fgets(buffer, sizeof(buffer), fichier) != NULL; ++i) {
        // Utiliser strtok pour diviser la ligne en tokens
        char *token = strtok(buffer, ",");
        matrice->vote[i] = (char **)malloc(colonne * sizeof(char *));

        for (int j = 0; j < colonne && token != NULL; ++j) {
            matrice->vote[i][j] = (char *)malloc((strlen(token) + 1) * sizeof(char));
            strcpy(matrice->vote[i][j], token);
            token = strtok(NULL, ",");
        }
    }

    fclose(fichier);
}

/************************************----------TABLEAU DES DONNEES DE VOTE----------*****************************************/
// Initialisation matrice
void initMatrice(t_mat_char_star_dyn *matrice, int ligne, int colonne) {
    matrice->ligne = ligne;
    matrice->colonne = colonne;

    // Allouer mémoire pour les pointeurs de lignes
    matrice->vote = (char ***)malloc(ligne * sizeof(char **));

    // Allouer mémoire pour chaque ligne
    for (int i = 0; i < ligne; i++)
        matrice->vote[i] = (char **)malloc(colonne * sizeof(char *));
}


// Libérer la mémoire allouée pour la matrice
void freeMatrice(t_mat_char_star_dyn *matrice) {
    // Libérer la mémoire pour chaque ligne
    for (int i = 0; i < matrice->ligne; ++i) {
        for (int j = 0; j < matrice->colonne; ++j) {
            free(matrice->vote[i][j]);
        }
        free(matrice->vote[i]);
    }
    // Libérer la mémoire pour les pointeurs de lignes
    free(matrice->vote);
}


/************************************----------MATRICE DES DUELS----------*****************************************/


MatriceDuel creerMatriceDuel(int lignes, int colonnes) {
    MatriceDuel matrice;
    matrice.lignes = lignes;
    matrice.colonnes = colonnes;

    matrice.donnees = (int **)malloc(lignes * sizeof(int *));
    if (matrice.donnees == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les lignes\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < lignes; ++i) {
        matrice.donnees[i] = (int *)malloc(colonnes * sizeof(int));
        if (matrice.donnees[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour les colonnes\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            matrice.donnees[i][j] = 0;
        }
    }

    return matrice;
}


void libererMatriceDuel(MatriceDuel *matrice) {
    for (int i = 0; i < matrice->lignes; ++i) {
        free(matrice->donnees[i]);
    }
    free(matrice->donnees);
}


void remplirMatriceDuel(const t_mat_char_star_dyn *matriceVote, MatriceDuel *matriceDuels) {
    // Tableau pour suivre les paires déjà comparées
    int dejaCompare[MAX_CANDIDATS][MAX_CANDIDATS] = {0};

    for (int i = 1; i < matriceVote->ligne; ++i) {
        // Réinitialiser le tableau dejaCompare à chaque nouveau vote
        memset(dejaCompare, 0, sizeof(dejaCompare));

        for (int j = 4; j <= 13; ++j) {
            for (int k = j + 1; k <= 13; ++k) {
                int classement_burger1 = atoi(matriceVote->vote[i][j]);
                int classement_burger2 = atoi(matriceVote->vote[i][k]);

                if (classement_burger1 != -1 && classement_burger2 != -1 &&
                    !dejaCompare[j - 4][k - 4] && !dejaCompare[k - 4][j - 4]) {

                    // Les deux burgers sont préférés (classement plus petit) ou non préférés (classement égal)
                    if (classement_burger1 < classement_burger2) {
                        matriceDuels->donnees[j - 4][k - 4]++;
                    } 
                    else if (classement_burger1 > classement_burger2) {
                        matriceDuels->donnees[k - 4][j - 4]++;
                    }

                    // Marquer cette paire comme déjà comparée
                    dejaCompare[j - 4][k - 4] = 1;
                    dejaCompare[k - 4][j - 4] = 1;
                }
            }
        }
    }
}




void afficherMatriceDuelAvecNoms(const MatriceDuel *matrice, int nbCandidats) {
    // Trouver la longueur maximale des noms
    int longueurMaxNom = 0;
    for (int i = 0; i < nbCandidats; ++i) {
        int longueurNom = strlen(candidats[i].nom);
        if (longueurNom > longueurMaxNom) {
            longueurMaxNom = longueurNom;
        }
    }

    // Afficher les noms des candidats en haut de la matrice
    printf("%-*s |", longueurMaxNom, "");

    for (int i = 0; i < nbCandidats; ++i) {
        printf(" %-*s |", longueurMaxNom, candidats[i].nom);
    }
    printf("\n");

    // Afficher une ligne de séparation
    for (int i = 0; i < nbCandidats + 1; ++i) {
        for (int j = 0; j < longueurMaxNom + 3; ++j) {
            printf("-");
        }
    }
    printf("\n");

    // Afficher la matrice avec les noms des candidats sur le côté
    for (int i = 0; i < matrice->lignes; ++i) {
        printf("%-*s |", longueurMaxNom, candidats[i].nom);

        for (int j = 0; j < matrice->colonnes; ++j) {
            printf(" %-*d |", longueurMaxNom, matrice->donnees[i][j]);
        }
        printf("\n");

        // Afficher une ligne de séparation après chaque ligne de données
        for (int k = 0; k < nbCandidats + 1; ++k) {
            for (int l = 0; l < longueurMaxNom + 3; ++l) {
                printf("-");
            }
        }
        printf("\n");
    }
}


/************************************----------GRAPHE----------*****************************************/

ListeArcs creerListeArcs() {
    ListeArcs liste;
    liste.debut = NULL;
    return liste;
}

void ajouterArc(ListeArcs *liste, int sommetDepart, int sommetArrivee, MatriceDuel * matriceDuels) {
    Arc *nouvelArc = (Arc *)malloc(sizeof(Arc));
    if (nouvelArc == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour un arc\n");
        exit(EXIT_FAILURE);
    }

    nouvelArc->sommetDepart = sommetDepart;
    nouvelArc->sommetArrivee = sommetArrivee;
    nouvelArc->poids = matriceDuels->donnees[sommetDepart][sommetArrivee] - matriceDuels->donnees[sommetArrivee][sommetDepart];
    nouvelArc->suivant = liste->debut;

    liste->debut = nouvelArc;
}


ListeArcs creerListeArcsDepuisMatrice(MatriceDuel matriceDuels, int nbCandidats) {
    ListeArcs liste = creerListeArcs();

    for (int i = 0; i < nbCandidats; ++i) {
        for (int j = 0; j < nbCandidats; ++j) {
            if (matriceDuels.donnees[i][j] > matriceDuels.donnees[j][i] && i!=j) 
                ajouterArc(&liste, i, j, &matriceDuels);
            
            else if (matriceDuels.donnees[i][j] < matriceDuels.donnees[j][i] && i!=j)
                ajouterArc(&liste, j, i, &matriceDuels);
                
        }
    }

    return liste;
}

void libererListeArcs(ListeArcs *liste) {
    Arc *courant = liste->debut;
    while (courant != NULL) {
        Arc *suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    liste->debut = NULL;
}

void afficherListeArcs(const ListeArcs *liste, const Candidat candidats[MAX_CANDIDATS]) {
    Arc *courant = liste->debut;

    while (courant != NULL) {
        printf("(%s -> %s, poids = %d)\n", candidats[courant->sommetDepart].nom, 
               candidats[courant->sommetArrivee].nom, courant->poids);
        courant = courant->suivant;
    }
}



int nombrePossibiltePermutation(int nb_candidat){
    int nb_permutation=nb_candidat;
    if (nb_candidat>0)
        return nb_permutation*nombrePossibiltePermutation(nb_candidat-1);
    else 
        return 1;
}



int main() {
    
    printf("nombre de possibilte de COMBINAISON = %d\n",nombrePossibiltePermutation(MAX_CANDIDATS));
    

    // Initialiser la matrice
    t_mat_char_star_dyn mat;
    initMatrice(&mat, LIGNES, COLONNES);

    // Charger le fichier CSV dans la matrice
    lecture_csv("VoteCondorcet.csv", &mat, LIGNES, COLONNES);

    // Afficher le contenu de la matrice à titre de test
    /*for (int i = 0; i < mat.ligne; ++i) {
        for (int j = 0; j < mat.colonne; ++j) {
            printf("%s ", mat.vote[i][j]);
        }
        printf("\n");
    }*/
    
    MatriceDuel matriceDuels = creerMatriceDuel(MAX_CANDIDATS, MAX_CANDIDATS);

    //remplir matrice des duels
    remplirMatriceDuel(&mat, &matriceDuels);

    // Affichage de la matrice des duels avec les noms des candidats
    afficherMatriceDuelAvecNoms(&matriceDuels, MAX_CANDIDATS);
    printf ("les duels se lisent en fonction des lignes ex :\n BBP a gagne 23 fois contre BSO\n et BSO 15 fois contre BBP\n"); 
    printf("\n");
    
    // Créer la liste des arcs à partir de la matrice des duels
    ListeArcs listeArcs = creerListeArcsDepuisMatrice(matriceDuels, MAX_CANDIDATS);

    // Afficher la liste des arcs
    afficherListeArcs(&listeArcs,candidats);
    

    // Libération de la mémoire
    freeMatrice(&mat);
    libererMatriceDuel(&matriceDuels);
    // Libérer la mémoire de la liste des arcs
    libererListeArcs(&listeArcs);

    return 0;
}



