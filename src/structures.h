/// \file structures.h
/// \brief Fichier de déclaration des structures utilisées dans le programme.
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 28 octobre 2023


#ifndef STRUCTURES_H
#define STRUCTURES_H

/// \def LONGUEUR_JUGEMENT
#define LONGUEUR_JUGEMENT 24

/// \def LONGUEUR_RESULTAT_JUGEMENT
#define LONGUEUR_RESULTAT_JUGEMENT 25

/// \def LONGUEUR_CONDORCET
#define LONGUEUR_CONDORCET 38

/// \def LONGUEUR_RESULTAT_CONDORCET
#define LONGUEUR_RESULTAT_CONDORCET 39

/// \def MAX_LINE_LENGTH
/// \brief Longueur maximale d'une ligne dans un fichier.
#define MAX_LINE_LENGTH 2048

/// \def MAX_VOTANTS
/// \brief Nombre maximal de votants.
#define MAX_VOTANTS 100


/// \def MAX_VOTES
/// \brief Nombre maximal de votes.
#define MAX_VOTES 100

/// \struct votant
/// \brief Structure représentant un votant.
/// Un votant est défini par son numéro étudiant et une clé.
typedef struct {
    int num_etu;
    char key[30];
} votant;

/// \struct vote
/// \brief Structure représentant un vote.
///
/// Un vote est défini par sa réponse, la date du vote, le cours associé, un hachage,
/// et le détail des votes par index de burger.
typedef struct {
    int reponse;
    char date[20];
    char cours[50];
    char hashed[64];
    int votes[10]; // votes [index burger]
} vote;


#endif // STRUCTURES_H
