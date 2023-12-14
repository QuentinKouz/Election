
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "structure_graphe.h"
#include <stdbool.h>

// schulzes **************************************

bool equals(Arc * a1, Arc * a2) {
    return (a1->sommetArrivee) == (a2->sommetArrivee) && (a1->sommetDepart) == (a2->sommetDepart);
}

bool estDedans(Arc * arc, ListeArcs * listeArcs) {
    Arc * e = listeArcs->debut;
    while(e!=NULL) {
        if(equals(e,arc)) {
            return true;
        }
        e = e->suivant;
    }
    return false;
}

int min(ListeArcs * listeArcs) {
    Arc * e = listeArcs->debut;
    int minimum = e->poids;
    while (e!=NULL) {
        if (e->poids < minimum){
            minimum = e->poids;
        }
        e = e->suivant;
    }
    return minimum;
}

ListeArcs *trouverCheminFortAetB(ListeArcs *graphe, int A, int B, ListeArcs * cheminFort) {    
    if (A==B) {
        printf("TEST\n");
        afficherListeArcs(cheminFort);
        return cheminFort;
    }

    Arc *arcCourant = graphe->debut;
    while (arcCourant != NULL) {
        if (arcCourant->sommetDepart == A && !estDedans(arcCourant, cheminFort)) {
            // faire une copie de l'arc courant
            Arc* arc = malloc(sizeof(arc));
            memcpy(arc, arcCourant, sizeof(Arc));
            arc->suivant = NULL;
            ajouterArc(cheminFort, arc);
            cheminFort->taille ++;
            ListeArcs * resultat = trouverCheminFortAetB(graphe, arcCourant->sommetArrivee, B, cheminFort);
            if (resultat != NULL) {
                return resultat;
            } else {
                retirerArc(cheminFort, arc);
            }
        }
        arcCourant = arcCourant->suivant;
    }

    return NULL;
}

void trouverCheminsForts(ListeArcs * graphe, ListeDeListeArcs * listeCheminsForts, int * listeCandidats, int nbCandidats) {
    for (int i=0; i<nbCandidats; i++) {
        for (int j=0; j<nbCandidats; j++) {
            if (i!=j) {
                ListeArcs* cheminFort = malloc(sizeof(ListeArcs));
                *cheminFort = creerListeArcs();
                cheminFort = trouverCheminFortAetB(graphe, listeCandidats[i], listeCandidats[j], cheminFort);
                cheminFort->poids = min(cheminFort);
                ajouterListeArc(listeCheminsForts, cheminFort);              
            }
        }
    }
    if (listeCheminsForts->debut == NULL) {
        printf("NULL\n");
    }
}

int trouverVainqueurSchulzes(ListeDeListeArcs listeCheminsForts, int nbCandidats) {
    // listeCheminsForts est rangé dans l'ordre (ex: 0>1, 0>2, 1>0, 1>2, 2>0, 2>1) -> nous profitons de cette propriété pour faciliter le calcul du vainqueur
    int tab_vainqueur[nbCandidats];
    for (int i=0; i<nbCandidats; i++) {
        tab_vainqueur[i] = 0;
        for (int j=0; j<nbCandidats-1; j++) {
            int decalage;
            if (j<i) {
                decalage = j*(nbCandidats-1) + (i-1);
            } else {
                decalage = (nbCandidats-1) * (j+1) + i;  
            }
            if (weight_at(&listeCheminsForts, (nbCandidats-1)*i+j) > weight_at(&listeCheminsForts, decalage)) {
                tab_vainqueur[i] += 1;
            }
        }
        if (tab_vainqueur[i] == nbCandidats-1) {
            return i;
        }
    }
    return -1;

}

// methode à appeler pour faire schulzes
void schulzes(ListeArcs * graphe) {
    ListeDeListeArcs listeCheminsForts = creerListeDeListeArcs();
    int candidats[] = {0, 1, 2, 3};
    trouverCheminsForts(graphe, &listeCheminsForts, candidats, 4);
    afficherListeDeListeArcs(&listeCheminsForts);
    int v = trouverVainqueurSchulzes(listeCheminsForts, 4);
    if (v==-1) {
        printf("ERREUR: Pas de vainqueur de schulzes\n");
        return;
    }
    printf("Le candidat n°%d est le vainqueur de schulzes !\n", v);
}

/* TEST SCHULZES
void remplirGraphe (ListeArcs * graphe) {
    Arc * a1 = malloc(sizeof(Arc));
    Arc * a2 = malloc(sizeof(Arc));
    Arc * a3 = malloc(sizeof(Arc));
    Arc * a4 = malloc(sizeof(Arc));
    Arc * a5 = malloc(sizeof(Arc));

    a1->sommetDepart = 0;
    a1->sommetArrivee = 1;
    a1->poids = 1;

    a2->sommetDepart = 1;
    a2->sommetArrivee = 2;
    a2->poids = 2;

    a3->sommetDepart = 2;
    a3->sommetArrivee = 0;
    a3->poids = 3;

    a4->sommetDepart = 3;
    a4->sommetArrivee = 2;
    a4->poids = 4;

    a5->sommetDepart = 0;
    a5->sommetArrivee = 3;
    a5->poids = 5;
    
    ajouterArc(graphe, a5);
    ajouterArc(graphe, a4);
    ajouterArc(graphe, a3);
    ajouterArc(graphe, a2);
    ajouterArc(graphe, a1);
}

int main(void) {
    ListeArcs graphe = creerListeArcs();
    remplirGraphe(&graphe);
    schulzes(&graphe);
}
*/
