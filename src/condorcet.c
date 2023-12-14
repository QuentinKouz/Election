/// \file condorcet.c
/// \brief Programme d'implémentation calculant le vainqueur de condorcet s'il existe
/// \author Equipe-2 : Salim Moussaoui, Maxime Cherraf, Quentin Kouzmitch, Nicolas Giry
/// \date 5 decembre 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "structure_graphe.h"
#include <stdbool.h>

//TODO : recuperer les noms depuis le fichiers csv 
char* noms[] = {"Burger Black Pepper", "Burger Sud-Ouest", "Thai Burger", "Veggie Burger", "Fire Cracker", "Roma", "Crispy" "Cheese Burger", "Burger Surprise", "Country"};

int calculerNbArcsMax(int nb_candidats){
    int nb_arcs = 0;
    for (int i=nb_candidats-1; i>0; i--) {
        nb_arcs += i;
    }
    return nb_arcs;
}

void trierGraphe(ListeArcs * graphe) {
    ListeArcs graphe_trie = creerListeArcs();
    int t = graphe->taille;
    for (int i=0; i<t; i++) {
        int poidsMax = graphe->debut->poids;
        Arc *arcVainqueur = graphe->debut;
        Arc *p = graphe->debut;
        while (p != NULL) {
            if (p->poids>poidsMax) {
                poidsMax = p->poids;
                arcVainqueur = p;
            }
            p = p->suivant;
        }
        ajouterArc(&graphe_trie, arcVainqueur);
        retirerArc(graphe, arcVainqueur);
    }
    graphe->debut = graphe_trie.debut;
}

bool creeCycle(ListeArcs *grapheCondorcet, Arc* arc) {
    // renvoie true si lors du parcours du graphe depuis le sommet destination de l'arc, on tombe sur le sommet source de l'arc
    int sommetDest = arc->sommetArrivee;
    int sommetDepart= arc->sommetDepart;

    if (estDansGraphe(grapheCondorcet, sommetDest)) {
        ListeArcs arcsAVerifier = creerListeArcs();
        ajouterArcSommet(grapheCondorcet, &arcsAVerifier, sommetDest);
        Arc* e = arcsAVerifier.debut;
        while (e!=NULL) {
            if (e->sommetArrivee == sommetDepart)
                // il y a un cycle
                return true;
            ajouterArcSommet(grapheCondorcet, &arcsAVerifier, e->sommetArrivee);
            e = e->suivant;
        }
    }
    return false;
}

void ajouterArcsSansCycle(ListeArcs *grapheCondorcet,ListeArcs* listeArcsTries) {
    Arc* arc = listeArcsTries->debut;
    while (arc!=NULL) {
        if(!creeCycle(grapheCondorcet, arc)) {
            ajouterArc(grapheCondorcet, arc);
        }
        arc = arc->suivant;
    }
}

bool estDansTab(int * tab, int sommet, int taille){
    for (int i=0; i<taille; i++){
        if (tab[i]==sommet)
            return true;
    }
    return false;
}

void retirer(int * tab, int sommet, int taille) {
    bool trouve = false;
    for (int i=0; i<taille-1; i++) {
        if (tab[i] == sommet) {
            trouve = true;
        }
        if (trouve) {
            tab[i] = tab[i+1];
        }
    }
}

int trouverVainqueurCondorcet(ListeArcs grapheCondorcet) {
    int candidats[MAX_CANDIDATS] ;
    Arc* e = grapheCondorcet.debut;
    int i=0;
    while (e!=NULL){
        if(!estDansTab(candidats, e->sommetDepart, i)){
            candidats[i] = e->sommetDepart;
            i++;
        }
        e = e->suivant;
    }
    e = grapheCondorcet.debut;
    while (e!=NULL){
        if(estDansTab(candidats, e->sommetArrivee, i)){
            retirer(candidats, e->sommetArrivee, i);
            i--;
        }
        e = e->suivant;
    }
    if (i==1){
        // il y a un vainqueur de condorcet :
        return candidats[0];
    }
    // pas de vainqueur de condorcet...
    return -1;
}

void obtenirGraphe(const char* fichier, ListeArcs * graphe) {
    // obtenir liste arcs et matrice duels
    t_mat_char_star_dyn mat;
    initMatrice(&mat, LIGNES, COLONNES);
    lecture_csv_score_condorcet(fichier, &mat, LIGNES, COLONNES);
    MatriceDuel matriceDuels = creerMatriceDuel(MAX_CANDIDATS, MAX_CANDIDATS);
    remplirMatriceDuel(&mat, &matriceDuels);
    creerListeArcsDepuisMatrice(matriceDuels, MAX_CANDIDATS, graphe);
}

void obtenirGraphePaire(const char* fichier, ListeArcs * graphe) {

    // obtenir liste arcs et matrice duels
    t_mat_char_star_dyn mat;
    initMatrice(&mat, LIGNES, COLONNES);
    lecture_csv_score_condorcet(fichier, &mat, LIGNES, COLONNES);
    MatriceDuel matriceDuels = creerMatriceDuel(MAX_CANDIDATS, MAX_CANDIDATS);
    remplirMatriceDuel(&mat, &matriceDuels);
    creerListeArcsDepuisMatrice(matriceDuels, MAX_CANDIDATS, graphe);

    // trier graphe
    trierGraphe(graphe);

    // ajout des arcs un par un sans cycle
    ListeArcs grapheCondorcet = creerListeArcs();
    ajouterArcsSansCycle(&grapheCondorcet, graphe);

    graphe = &grapheCondorcet;
}

void testCreationSansCycle() {
    ListeArcs grapheCondorcet = creerListeArcs();
    Arc a, b, c, d, e;
    a.sommetDepart = 1;
    a.sommetArrivee = 0;
    a.poids = 10;
    
    b.sommetDepart = 0;
    b.sommetArrivee = 2;
    b.poids = 9;
    
    c.sommetDepart = 2;
    c.sommetArrivee = 1;
    c.poids = 8;
    
    d.sommetDepart = 3;
    d.sommetArrivee = 1;
    d.poids = 7;
    
    e.sommetDepart = 0;
    e.sommetArrivee = 2;
    e.poids = 6;

    ajouterArc(&grapheCondorcet, &a);
    ajouterArc(&grapheCondorcet, &b);
    ajouterArc(&grapheCondorcet, &c);
    ajouterArc(&grapheCondorcet, &d);
    //ajouterArc(&grapheCondorcet, &e);

    ListeArcs graphe2 = creerListeArcs();

    ajouterArcsSansCycle(&graphe2, &grapheCondorcet);
    afficherListeArcs(&graphe2);
}

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <fichier_condorcet>\n", argv[0]);
        exit(2);
    }

    // renvoie un tableau d'arc représentant les résultats des duels entre chaque candidats
    ListeArcs grapheCondorcet = creerListeArcs();
    obtenirGraphe(argv[1], &grapheCondorcet);
    
    int vainqueur = trouverVainqueurCondorcet(grapheCondorcet);

    printf("Mode de scrutin : Condorcet\n");

    if (vainqueur == -1) {
        printf("PAS DE VAINQUEUR DE CONDORCET ...\n");
    }
    else {
        printf("\tLe vainqueur de condorcet est : %s\n", noms[vainqueur]);
    }
    return 0;
}