#include "sha256.h"
#include "sha256_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"



#define AUCUN 0
#define JUGEMENT 1
#define CONDORCET 2


// Vérifier si la personne est bien inscrite dans le fichier des électeurs
// Check si le num étudiant est bien dans le fichier ainsi que la clé
// Puis check si la clé et le num étudiant sont sur la même ligne
int verifier_votant(char fichier[], int longueur, votant * votants[], int numero_etudiant, char key[], int methode){
  lecture_csv(fichier,&votants);
  for (int ligne = 0; ligne < longueur; ligne++){
    if ((votants[ligne]->num_etu == numero_etudiant) && (strcmp(key,votants[ligne]->key) == 0)){
        return methode;
    }
  }
  return AUCUN;
}


/* La fonction retournera :
  - 0 s'il n'est aucun fichier
  - 1 s'il est inscrit dans JUGEMENT
  - 2 s'il est inscrit dans CONDORCET
  - 3 s'il est inscrit dans JUGEMENT et CONDORCET*/
int main(int argc, char * argv[]) {
    SHA256_CTX sha;
    int len_data;
    BYTE data; // clé
    BYTE * hash;
    char fichier_votant_jugement[] = "ResultatsVote/codeJugementNumEtu.csv";
    char fichier_votant_condorcet[] = "ResultatsVote/codeCondorcetNumEtu.csv";
    votant votant_jugement[LONGUEUR_JUGEMENT];
    votant votant_condorcet[LONGUEUR_CONDORCET];
    int type_vote = AUCUN;
    char fichier_resultat_jugement[] = "ResultatsVote/jugement.csv";
    char fichier_resultat_condorcet[] = "ResultatsVote/VoteCondorcet.csv";


    // Vérification du bon nombre d'argument (Gestion d'erreur à l'appel)
    if (argc < 3) {
        fprintf(stderr, "Usage: ./verify_my_vote + numéro étudiant + code secret\n");
        exit(1);
    }

    int numero_etudiant = atoi(argv[1]);
    char key[] = argv[2];

    // On vérifie si la personne est dans la liste des votants de Jugement
    type_vote += verifier_votant(fichier_votant_jugement,LONGUEUR_JUGEMENT, &votant_jugement, numero_etudiant, key, JUGEMENT);
    // On vérifie si la personne est dans la liste des votants de Condorcet
    type_vote += verifier_votant(fichier_votant_condorcet,LONGUEUR_CONDORCET, &votant_condorcet, numero_etudiant, key, CONDORCET);

    // Si n'est inscrit dans aucun vote, on retourne 0
    if (type_vote == AUCUN){
      printf("Vous n'êtes dans la liste correspondant à cette clé.\n");
      return AUCUN;
    }



    // on crée la structure des votes en fonction du type de vote puis on l'initialise
    if (type_vote == JUGEMENT) {
      nb_lignes_vote = LONGUEUR_RESULTAT_JUGEMENT;
      vote votes[nb_lignes_vote];
      lecture_csv(fichier_resultat_jugement, &votes);
    } else if (type_vote == CONDORCET){
      nb_lignes_vote = LONGUEUR_RESULTAT_CONDORCET;
      vote votes[nb_lignes_vote];
      lecture_csv(fichier_resultat_condorcet, &votes);
    } else {
      printf("Il y a une erreur lors de la vérification\n");
      exit(1);
    }

    // on crée le hash depuis la clé de votant pour la retrouver en suivant
    data = argv[2];
    len_data = strlen(data);
    sha256_init(&sha);
    sha256_update(&sha, data, len_data);
    sha256_final(&sha, &hash);


    // On regarde dans les votes à quelle ligne se situe le vote du votant
    int ligne_resultat_vote =0;
    while (ligne_resultat_vote<nb_lignes_vote && vote[ligne_resultat_vote].hashed != hash) {
        ligne_resultat_vote++;
    }
    if (votes[ligne_resultat_vote].hashed != hash){
      printf("Il y a une erreur le hash de votre vote n'est pas dans les votes\n");
      exit(1);
    }

    // Affichage du vote
    if (type_vote == JUGEMENT) {
      printf("Votre vote se situe à la ligne %d du fichier JUGEMENT.\n", ligne_resultat_vote);
    } else if (type_vote == CONDORCET){
      printf("Votre vote se situe à la ligne %d du fichier CONDORCET.\n", ligne_resultat_vote);
    } else {
      printf("Il y a une erreur lors de la vérification\n");
      exit(3);
    }

    return 0;
}
