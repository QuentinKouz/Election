/// \file uninominal1.c

/// \brief Programme d'implémentation de la méthode uninominal à un tour.

/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry

/// \date 9 novembre 2023



#include <stdio.h>

#include <stdlib.h>

#include <string.h>







#define MAX_CANDIDATS 10

#define MAX_VOTANTS 100

#define TAILLE_MAX_LIGNE 1024







/**

 * \struct Candidat

 * \brief Structure pour stocker les informations des candidats.

 */



typedef struct {

    char nom[50];

    int score;

} Candidat;



/**

 * \brief Lit et ignore les en-têtes d'un fichier CSV.

 * 

 * \param[in] fichier Pointeur vers le fichier CSV à traiter.

 */

void ignorerEnTetes(FILE *fichier) {

    char buffer[TAILLE_MAX_LIGNE];

    fgets(buffer, TAILLE_MAX_LIGNE, fichier);

}



/**

 * \brief Trouve le candidat vainqueur.

 * \param[in] candidats Tableau des candidats.

 * \param[in] nbCandidats Nombre total de candidats dans le tableau.

 * \return Candidat ayant le score le plus élevé.

 */

Candidat trouverVainqueur(Candidat candidats[], int nbCandidats) {

    Candidat vainqueur = candidats[0];

    for (int i = 1; i < nbCandidats; i++) {

        if (candidats[i].score > vainqueur.score) {

            vainqueur = candidats[i];

        }

    }

    return vainqueur;

}



/**

 * \brief Calcule le score de votes pour un candidat.

 * \param[in] vote Nombre de votes pour le candidat.

 * \param[in] totalVotes Nombre total de votes exprimés.

 * \return Pourcentage des votes pour le candidat.

 */

float calculerScore(int vote, int totalVotes) {

    return ((float)vote / (float)totalVotes) * 10;

}



/**

 * \brief Analyse un fichier CSV pour déterminer le vainqueur du vote.

 *

 * \param[in] nomFichier Nom du fichier CSV contenant les votes.

 */

void determinerVainqueur(const char *nomFichier) {

    char cheminComplet[100];

    sprintf(cheminComplet, "ResultatsVote/%s", nomFichier);

    

    FILE *fichier = fopen(cheminComplet, "r");



    if (!fichier) {

        printf("Erreur d'ouverture du fichier %s\n", cheminComplet);

        exit(1);

    }

    

    ignorerEnTetes(fichier);



    Candidat candidats[MAX_CANDIDATS] = {0};

    strcpy(candidats[0].nom, "Burger Black Pepper");

    strcpy(candidats[1].nom, "Burger Sud-Ouest");

    strcpy(candidats[2].nom, "Thai Burger");

    strcpy(candidats[3].nom, "Veggie Burger");

    strcpy(candidats[4].nom, "Fire Cracker");

    strcpy(candidats[5].nom, "Roma");

    strcpy(candidats[6].nom, "Crispy");

    strcpy(candidats[7].nom, "Cheese Burger");

    strcpy(candidats[8].nom, "Burger Surprise");

    strcpy(candidats[9].nom, "Country");



    int nbVotants = 0;

    char buffer[TAILLE_MAX_LIGNE];



    while (fgets(buffer, TAILLE_MAX_LIGNE, fichier)) {

        int votes[MAX_CANDIDATS] = {0};

        char *token = strtok(buffer, ",");



        for (int i = 0; i < 4; i++) { // On ignore les 4 premières colonnes.

            token = strtok(NULL, ",");

        }



        for (int i = 0; i < MAX_CANDIDATS; i++) {

            if (token != NULL) {

                votes[i] = atoi(token);

                candidats[i].score += votes[i];

                token = strtok(NULL, ",");

            }

        }

        nbVotants++;

    }



    fclose(fichier);

    Candidat vainqueur = trouverVainqueur(candidats, MAX_CANDIDATS);

    float score = calculerScore(vainqueur.score, nbVotants);

    printf("Mode de scrutin : uninominal à un tour, %d candidats, %d votants, vainqueur = %s, score = %.2f%%\n", MAX_CANDIDATS, nbVotants-1, vainqueur.nom, score);

}



/// \brief Fonction principale du programme.

/// \param[in] argc : Le nombre d'arguments.

/// \param[in] argv : Le tableau des arguments.

/// \return 0 si le programme se termine avec succès, sinon un code d'erreur.

int main(int argc, char *argv[]) {



    if (argc != 3) {

        printf("Usage: %s <fichier_condorcet> <fichier_jugement>\n", argv[0]);

        exit(2);

    }



    determinerVainqueur(argv[1]);

    determinerVainqueur(argv[2]);



    return 0;



}
