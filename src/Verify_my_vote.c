#include "sha256.h"
#include "sha256_utils.h"
#include <stdlib.h>
#include "structure.h"

int main(int argc, char * argv[]) {
    SHA256_CTX sha;
    int len_data;
    BYTE data; // clé 
    BYTE hash[];

    if (argc < 2) {
        fprintf(srderr, "Usage: ./verify_my_vote key\n");
        exit(1);
    }

    data = argv[1];

    // lecture csv à faire
    vote votes[nb_lignes];

    // creer le hash depuis la clé de votant 
    len_data = len(data);

    sha256_init(&sha);
    sha256_update(&sha, data, len_data);
    sha256_final(&sha, &hash);
    

    // regarder dans votes les votes de chacun
    int i=0;
    while (i<nb_lignes && votes[i].hashed != hash) {
        i++;
    }

    // afficher votes[i]
    return 0;
}