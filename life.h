/*
// Created by Legadus_Pro on 07/08/2026.
*/

#ifndef GAMEOFLIFE_LIFE_H
#define GAMEOFLIFE_LIFE_H

/**
 * Dimensions fixes du plateau de jeu.
 * Définies via #define (et non comme champs de la struct Board) car un
 * tableau de taille fixe (int board[width][height]) doit avoir une taille
 * connue à la compilation. Le préprocesseur remplace ces macros par des
 * constantes numériques avant même que le compilateur ne lise la struct,
 * évitant ainsi le problème "un champ dépend d'un autre champ du même struct".
 */
#define width 4
#define height 4

/**
 * @brief Représente l'état complet d'un plateau du Jeu de la Vie.
 *
 * board[i][j] vaut :
 *   - 1 si la cellule à la position (i, j) est vivante
 *   - 0 si la cellule à la position (i, j) est morte
 *
 * Le plateau est de taille fixe (width x height), stocké en mémoire
 * contiguë (utile notamment pour comparer deux plateaux avec memcmp,
 * ou pour comprendre pourquoi board[i][j] != c->board[i][j] compare
 * du contenu et non des adresses).
 */
typedef struct {
    int board[width][height];
} Board;

/**
 * @brief Génère aléatoirement une valeur de cellule (vivante ou morte).
 *
 * @return 1 (vivante) ou 0 (morte), selon un tirage aléatoire.
 *
 * @note Dépend de rand(). La graine (srand) doit être initialisée une
 *       seule fois dans main(), AVANT le premier appel à cette fonction
 *       (typiquement avant l'appel à random_board()) — ne pas appeler
 *       srand() ici, sous peine d'obtenir la même valeur à chaque appel
 *       si plusieurs appels ont lieu dans la même seconde.
 */
int randomNum(void);

/**
 * @brief Alloue et initialise un plateau vide (toutes les cellules mortes).
 *
 * @return Pointeur vers un nouveau Board alloué dynamiquement (malloc),
 *         ou NULL si l'allocation échoue.
 *
 * @warning L'appelant est responsable de libérer la mémoire retournée
 *          avec free() lorsque le plateau n'est plus utilisé.
 */
Board* initBoard(void);

/**
 * @brief Alloue et initialise un plateau avec des valeurs aléatoires.
 *
 * Parcourt chaque cellule du plateau et lui assigne une valeur via
 * randomNum().
 *
 * @return Pointeur vers un nouveau Board alloué dynamiquement (malloc),
 *         ou NULL si l'allocation échoue.
 *
 * @warning L'appelant est responsable de libérer la mémoire retournée
 *          avec free() lorsque le plateau n'est plus utilisé.
 */
Board* random_board(void);

/**
 * @brief Affiche l'état actuel du plateau dans la console.
 *
 * @param b Pointeur vers le Board à afficher. Ne doit pas être NULL.
 */
void renderBoard(Board* b);

/**
 * @brief Calcule le prochain état du plateau selon les règles du Jeu de la Vie.
 *
 * Pour chaque cellule, compte ses 8 voisins vivants (en excluant la
 * cellule elle-même, et en respectant les bords du plateau), puis
 * applique les règles :
 *   - une cellule vivante avec 2 ou 3 voisins vivants reste vivante
 *   - une cellule vivante avec moins de 2 ou plus de 3 voisins meurt
 *   - une cellule morte avec exactement 3 voisins vivants devient vivante
 *
 * @param b Pointeur vers le Board actuel (état de lecture, non modifié).
 * @return Pointeur vers un NOUVEAU Board alloué dynamiquement représentant
 *         l'état suivant. b n'est pas modifié : la fonction lit depuis b
 *         et écrit dans un plateau séparé, afin d'éviter que des cellules
 *         déjà mises à jour n'influencent le comptage des voisins restants.
 *
 * @warning L'appelant est responsable de libérer la mémoire retournée
 *          avec free() lorsque ce plateau n'est plus utilisé.
 */
Board* nextState(Board* b);

/**
 * @brief Compare deux plateaux cellule par cellule.
 *
 * @param b Premier plateau à comparer.
 * @param c Second plateau à comparer.
 * @return 1 si les deux plateaux sont identiques (toutes les cellules
 *         correspondantes sont égales), 0 s'il existe au moins une
 *         différence.
 *
 * @note Utilise un "early return" : la fonction s'arrête dès qu'une
 *       différence est trouvée, sans parcourir le reste du plateau.
 */
int compare(Board* b, Board* c);

/**
 * @brief Fait évoluer et affiche le plateau génération après génération,
 *        jusqu'à ce qu'il se stabilise (deux générations consécutives
 *        identiques).
 *
 * Boucle : calcule la génération suivante, l'affiche, puis avance d'une
 * génération, jusqu'à détecter un état stable (compare() renvoie 1).
 * Si le plateau contient un motif oscillant (ex: un "blinker"), la
 * boucle peut continuer indéfiniment, car deux générations consécutives
 * ne seront jamais identiques.
 *
 * @param b Pointeur vers le Board initial.
 *
 * @warning Cette fonction PREND POSSESSION du plateau passé en paramètre
 *          et libère (free) chaque génération intermédiaire, y compris
 *          b lui-même, au fil de son exécution. L'appelant ne doit plus
 *          utiliser b après avoir appelé infiniteLife(b).
 */
void infiniteLife(Board* b);

#endif /*GAMEOFLIFE_LIFE_H*/
