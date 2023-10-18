typedef struct {
    int num_etu;
    char[] key;
} votant;

typedef struct {
    int reponse;
    char[] date;
    char[] cours;
    char[] hashed;
    int[][] votes; // votes [index burger] [notes burger]
} vote;