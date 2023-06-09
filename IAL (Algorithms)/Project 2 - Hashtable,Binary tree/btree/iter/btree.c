/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (tree == NULL) 
        return false;
    while (true)
    {
        if ((tree)->key == key)
            return true;
        if (tree->key > key && tree->left != NULL)
            tree = tree->left;
        else if (tree->key < key && tree->right != NULL)
            tree = tree->right;
        else
            return false;
    }
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if (*tree == NULL)
    {
        (*tree) = malloc (sizeof(bst_node_t));
        if (tree == NULL)
            return;
        **tree = (bst_node_t){.key = key, .value = value, .left = NULL, .right = NULL};
    }
    else
    {
        bst_node_t * prev = NULL;
        bst_node_t * next = (*tree);
        while ((next) != NULL)
        {
            prev = next;
            if ((next)->key == key)
            {
                (next)->value = value;
                return;
            }
            else if ((next)->key > key)
            {
                (next) = (next)->left;
            }
            else if ((next)->key < key)
            {
                (next) = ((next)->right);
            }

        }
        bst_node_t * node = malloc (sizeof(bst_node_t));
        if(node == NULL)
            return;
        *node = (bst_node_t){.key = key, .value = value, .left = NULL, .right = NULL};
        if ((prev)->key > key){
            (prev)->left = node;}
        else 
            (prev)->right = node;
    }

}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t * next = (*tree);
    bst_node_t * prev = NULL;
    while (next->right != NULL)
    {
        prev = next;
        next = next->right;
    } 
    target->key = next->key;
    target->value = next->value;
    if (next->left != NULL)
    {
        bst_node_t * temp = next;
        *next = *next->left;
        free(temp);
    }
    else if (prev != NULL)
    {
        prev->right = NULL;
        free(next);
    }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (*tree == NULL)
        return;
    bst_node_t * toDel = NULL;
    bst_node_t * next = (*tree);
    bool leftOrRight = false; //left
    while (toDel == NULL && next != NULL) 
    {
        if ((next)->key == key)
            toDel = (next);
        else if (next->left && next->left->key == key)
            {
                toDel = next->left;
                leftOrRight = false;
            }
        else if (next->right && next->right->key == key)
            {
                toDel = next->right;
                leftOrRight = true;
            }
        else
        {
            if (next->key > key)
                next = next->left;
            else
                next = next->right;

        }
    }
    if (toDel != NULL) 
    {
        if (toDel->left == NULL && toDel->right != NULL)
            {
                if (leftOrRight)
                    next->right = toDel->right;
                else 
                    next->left = toDel->right;
            }
        else if (toDel->right == NULL && toDel->left != NULL)
            {

                if (leftOrRight)
                    next->right = toDel->left;
                else 
                    next->left= toDel->left;

            }
        else if (toDel->left == NULL && toDel->right == NULL)
        {
            if (next->left && next->left->key == key)
                next->left = NULL;
            else if (next->right && next->right->key == key)
                next->right = NULL;
            else 
                next = NULL;
        }
        else 
        {
            bst_replace_by_rightmost(toDel, &(toDel)->left);
            return;
        }
        free(toDel);
        
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    do{
        if ((*tree) == NULL)
        {
            if (!stack_bst_empty(&stack))
            {
                (*tree) = stack_bst_top(&stack);
                stack_bst_pop(&stack);
            }
        }
        else 
        {
            if ((*tree)->right != NULL)
            {
                stack_bst_push(&stack, (*tree)->right);
            }
            bst_node_t * auxPtr = (*tree);
            (*tree) = (*tree)->left;
            free(auxPtr);
        }
    } while (((*tree) != NULL) || (!stack_bst_empty(&stack)));
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL)
    {
        stack_bst_push(to_visit, tree);
        bst_print_node(tree);
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_leftmost_preorder(tree, &stack);
    while (!stack_bst_empty(&stack))
    {
        tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_leftmost_preorder(tree->right, &stack);
    }

}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while ((tree) != NULL)
    {
        stack_bst_push(to_visit, (tree));
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_leftmost_inorder(tree, &stack);
    while (!stack_bst_empty(&stack))
    {
        tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_print_node(tree);
        bst_leftmost_inorder(tree->right, &stack);
    }

}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    while (tree != NULL)
    {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;

    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    bool fromLeft;
    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bool_t stackB;
    stack_bool_init(&stackB);
    bst_leftmost_postorder(tree, &stack, &stackB);
    while (!stack_bst_empty(&stack))
    {
        tree = stack_bst_top(&stack);
        fromLeft = stack_bool_top(&stackB);
        stack_bool_pop(&stackB);
        if (fromLeft)
        {
            stack_bool_push(&stackB, false);
            bst_leftmost_postorder(tree->right, &stack, &stackB);
        }
        else
        {
            stack_bst_pop(&stack);
            bst_print_node(tree);
        }
    }

}
