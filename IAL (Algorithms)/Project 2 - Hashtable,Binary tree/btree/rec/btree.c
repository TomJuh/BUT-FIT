/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
void bst_init(bst_node_t **tree) { //sets to root to NULL
    (*tree)=NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (tree != NULL) 
    {
        if (tree->key == key) //tree is found
        {
            *value = tree->value;
            return true;
        }
        else if (tree->left != NULL && key < tree->key) //if left node isn't empty
            bst_search(tree->left, key, value); //call recursively for left node
        else if (tree->right != NULL)
            bst_search(tree->right, key, value);
    }
  return false;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if (*tree == NULL) //if tree is empty
    {
        (*tree) = malloc (sizeof(bst_node_t));
        if(tree == NULL)
            return;
        **tree = (bst_node_t){.key = key, .value = value, .left = NULL, .right = NULL};

        return ;
    }
    if ((*tree)->key == key) //rewrite value if key is already used
    {
         (*tree)->value= value;
         return ;
    }
    else 
    {
        if ((*tree)->key > key && (*tree)->left !=  NULL) //if key is smaller and left node exists
                bst_insert(&(*tree)->left, key, value); //call recursively for left node
            
        else if ((*tree)->key < key && (*tree)->right != NULL) //if key is larger call right node
                bst_insert(&(*tree)->right, key, value);
        else
        { //create new node
                bst_node_t * node = malloc (sizeof(bst_node_t));
                if(node == NULL)
                    return;
                *node = (bst_node_t){.key = key, .value = value, .left = NULL, .right = NULL};
                if ((*tree)->key > key) //if key is smaller assign to left node
                    (*tree)->left = node;
                else //if larger assign to right node
                    (*tree)->right = node;
        }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) { //REDO
    if ((*tree)->right != NULL) //until the rightmost node is found call recursively
        bst_replace_by_rightmost(target, &(*tree)->right);
    else
    {   //reassign value to target
        target->key = (*tree)->key;
        target->value = (*tree)->value; 

        if ((*tree)->left != NULL) //assign left node children to parrent
        {
            bst_node_t * temp = ((*tree));
            *(tree)  = temp->left;
            free (temp);
        }
        else 
        {

            free((*tree));
            (*tree) = NULL;
        }
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t * toDel = NULL;
    bool assignToRight = false; 
    if (*tree == NULL)
        return;
    if ((*tree)->key == key) //node to delete is found
        toDel = (*tree);
    else if (((*tree)->left && (*tree)->left->key == key)){//node to delete is left children
        toDel = (*tree)->left;
        assignToRight = false;
    }
    else if ((*tree)->right &&(*tree)->right->key == key) { //node to delete is right children
        toDel = (*tree)->right;
        assignToRight = true;
    }
    if (toDel != NULL)
    {
        if (toDel->left == NULL && toDel->right != NULL)//if toDel node has right node
        {
            if (assignToRight) //assign to original parrent
                (*tree)->right = toDel->right;
            else 
                (*tree)->left = toDel->right;
        }
        else if (toDel->right == NULL && toDel->left != NULL)
        {
            if (assignToRight)
                (*tree)->right = toDel->left;
            else 
                (*tree)->left = toDel->left;

        }
        else if (toDel->left == NULL && toDel->right == NULL) //if both are NULL fix parrent pointers
        {
            if ((*tree)->left && (*tree)->left->key == key)
                (*tree)->left = NULL;
            else if ((*tree)->right&& (*tree)->right->key == key)
                (*tree)->right = NULL;
            else
                (*tree) = NULL;
        }
        else //if both children are not NULL
        {
            bst_replace_by_rightmost(toDel, &(toDel->left)); //replace node
            return;
        }

        free(toDel);
        return;

    }
    else if ((*tree)->left != NULL || (*tree)->right != NULL) //if key is not found and tree has children
        bst_delete(((*tree)->key > key)? &(*tree)->left: &(*tree)->right, key);//call recursively 
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if (*tree == NULL)
        return;
    bst_dispose(&(*tree)->left); //call recursively for left node
    bst_dispose(&(*tree)->right); //and for right node
    free(*tree); //frees parrent
    *tree = NULL;
    }

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    bst_print_node(tree);
    if ((*tree).left != NULL)
        bst_preorder((*tree).left);
    if ((*tree).right != NULL)
        bst_preorder((*tree).right);
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    if ((tree) != NULL)
    {
        bst_inorder(tree->left);
        bst_print_node(tree);
        bst_inorder(tree->right);
    }
    
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    if (tree != NULL)
    {
        bst_postorder((*tree).left);
        bst_postorder((*tree).right);
        bst_print_node(tree);
    }

}
