/*
// Created by Legadus_Pro on 07/08/2026.
*/

#include "life.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


/*
 * Tire une valeur de cellule (vivante/morte) avec ~50% de chances chacune.
 *
 * rand() % 10 donne un nombre entre 0 et 9 (10 valeurs possibles,
 * réparties uniformément par rand()). Sur ces 10 valeurs, exactement
 * 5 sont paires (0, 2, 4, 6, 8) : n % 2 == 0 a donc ~50% de chances
 * d'être vrai, contrairement à une condition du type "n != 0" qui
 * biaiserait le résultat à 90% de cellules vivantes.
 *
 * NE PAS appeler srand() ici : cette fonction est appelée une fois par
 * cellule (donc width*height fois par plateau). La graine doit être
 * fixée UNE SEULE FOIS dans main(), avant le premier appel à
 * random_board(), sinon plusieurs appels dans la même seconde produiraient
 * tous la même valeur (time(NULL) a une résolution d'une seconde).
 */
int randomNum(void) {
     int n =  rand()%10;
     if ( n%2 == 0) {
          return 1;
     }
     return 0;
}

/*
 * Alloue un plateau et met toutes ses cellules à 0 (mortes) d'un coup.
 * calloc (contrairement à malloc) initialise la mémoire allouée à zéro,
 * ce qui évite d'avoir à boucler manuellement pour "vider" le plateau.
 */
Board* initBoard(void) {
     Board* new = (Board*)calloc(1, sizeof(Board));
     return new;
}


/*
 * Alloue un plateau et remplit chaque cellule via randomNum().
 * malloc() ne met pas la mémoire à zéro : chaque cellule est donc
 * explicitement écrite dans la boucle, pas seulement les cellules
 * "vivantes" comme on pourrait le faire avec calloc.
 */
Board* random_board( void) {

     Board* b = (Board*)malloc(sizeof(Board));
     if (b == NULL) {
          return NULL;
     }else {
          int i, j;
          for (i = 0; i < width; i++) {
               for ( j = 0; j < height; j++) {
                    b->board[i][j] = randomNum();
               }
          }
     }
     return b;
}

/*
 * Affiche le plateau ligne par ligne : "# " pour une cellule vivante,
 * "0" pour une cellule morte.
 */
void renderBoard(Board* b) {
     int i, j;
     for (i = 0; i < width; i++) {
          printf("----------\n");
          for (j = 0; j < height; j++) {
               if (b->board[i][j]== 1) {
                    printf("| # |");
               }else {
                    printf("| 0 |");
               }

          }
          printf("\n");
     }
}

/*
 * Calcule le plateau suivant selon les règles du Jeu de la Vie.
 *
 * Écrit dans un NOUVEAU plateau (new), jamais dans b : si on modifiait
 * b directement pendant qu'on le parcourt, les cellules déjà mises à
 * jour fausseraient le comptage des voisins des cellules suivantes
 * (ex: la cellule (1,0) verrait la valeur DÉJÀ MISE À JOUR de (0,0)
 * au lieu de sa valeur d'origine).
 */
Board* nextState(Board* b) {
     int i, j, x ,y;
     Board* new = (Board*)malloc(sizeof(Board));

     for (i = 0; i < width; i++) {
          for (j = 0; j < height; j++) {
               int count = 0;

               /*
                * Parcourt le bloc 3x3 centré sur (i,j) : x et y vont
                * de -1 à +1 autour de la cellule. Trois conditions
                * combinées dans le même if :
                *  - bornes du plateau (0 <= x < width, 0 <= y < height)
                *    pour ne jamais accéder hors tableau sur les bords
                *  - !(x == i && y == j) pour exclure la cellule
                *    elle-même (elle n'est pas son propre voisin) ;
                *    négation groupée plutôt que (x!=i && y!=j), qui
                *    exclurait à tort les voisins alignés en ligne/colonne
                *  - b->board[x][y] > 0 pour ne compter que les voisins
                *    vivants
                */
                for (x = i-1; x < i +2; x++) {
                     for (y = j-1; y < j+2; y++) {
                          if (  0 <= x  && x < width && 0 <= y && y < height && !(x == i && y == j) && b->board[x][y] > 0) {
                               count++;
                          }
                     }
                }

               /* Règles du Jeu de la Vie appliquées au nouveau plateau */
               if ( count < 2 || count > 3 ) {
                      new->board[i][j] = 0;
               }
               if ( (count < 4 && count > 1) || count == 3) {
                    new->board[i][j] = 1;
               }
          }
     }
     return new;
}

/*
 * Compare deux plateaux cellule par cellule (pas une comparaison
 * d'adresses comme le ferait b->board != c->board).
 *
 * Early return dès la première différence trouvée : pas besoin de
 * parcourir le reste du plateau une fois qu'on sait qu'ils diffèrent.
 * index reste à 1 (valeur par défaut) uniquement si la boucle se
 * termine sans jamais avoir déclenché ce return.
 */
int compare(Board* b, Board* c) {
     int i , j ;
     int index = 1;
     for ( i = 0; i < width; i++) {
          for ( j = 0; j < height; j ++) {
                if (b->board[i][j] != c->board[i][j]) {
                    return 0;
               }
          }
     }
    return index;
}

/*
 * Fait évoluer le plateau génération par génération jusqu'à stabilité
 * (deux générations consécutives identiques). S'arrête indéfiniment
 * en boucle si le plateau contient un oscillateur (ex: un "blinker"),
 * puisque dans ce cas deux générations consécutives ne sont jamais
 * identiques.
 *
 * Gestion mémoire (ordre important, tracé et corrigé en session) :
 *   1. free(b) libère l'ANCIEN plateau, celui qui vient de servir à
 *      calculer "next" et qu'on n'affichera plus jamais directement.
 *      Ceci arrive AVANT que b ne soit réassigné.
 *   2. b = next fait pointer b vers le plateau qu'on vient d'afficher.
 *   3. next = nextState(b) alloue le plateau de la génération suivante.
 * Si l'ordre était inversé (free après réassignation), on libérerait
 * le plateau encore utilisé par le prochain compare(b, next) au tour
 * suivant → use-after-free.
 *
 * Cette fonction prend possession du plateau passé en paramètre :
 * il est libéré au fil de l'exécution, l'appelant ne doit plus
 * l'utiliser après avoir appelé infiniteLife(b).
 */
void infiniteLife(Board* b) {
     Board* next = nextState(b);
     while(compare(b,next)!= 1) {
          renderBoard(next);
          free(b);
          printf("##################################\n");
          b =  next;
          next = nextState(b);
     }
     renderBoard(next);
     free(b);
     free(next);
}