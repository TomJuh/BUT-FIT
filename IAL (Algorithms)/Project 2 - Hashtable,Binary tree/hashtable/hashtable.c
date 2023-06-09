/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) { //sets every item to NULL
    for (int index = 0; index < HT_SIZE; index++)
        (*table)[index] = NULL;
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    int index = get_hash(key);
    ht_item_t * srchItem = (*table)[index];
    while (srchItem != NULL){ //loop until equal key is found
        if (strcmp(srchItem->key, key) == 0)
            return srchItem;
        srchItem = srchItem->next;
    }
      return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    int index = get_hash(key);
    if (ht_search(table,key)) //rewrite value if key is already used
        ht_search(table, key)->value = value;
    else 
    {   //create and assign new item
        ht_item_t * nItem = malloc (sizeof(ht_item_t));
        *nItem = (ht_item_t) {.key = key, .value = value, .next = (*table)[index]};
        (*table)[index] = nItem;
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    if (ht_search(table, key))
        return &ht_search(table, key)->value;
    return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
//redo
void ht_delete(ht_table_t *table, char *key){
    int index = get_hash(key);
    ht_item_t * delItem = (*table)[index]; 
    ht_item_t * prvItem = delItem;
        while (delItem != NULL) //loop until key is found
        {
            if (strcmp(delItem->key, key) == 0)
            { 
                //assign children of deleted item to it's parrent
                prvItem->next = delItem->next; 
                break;
            }
            prvItem = delItem;
            delItem = delItem->next;
        }
    free(delItem);
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
    for (int index = 0; index < HT_SIZE && (*table)[index] != NULL; index++) //for every index
    {
        ht_item_t * delItem = (*table)[index];
        ht_item_t * temp;
        while (delItem != NULL) //remove and move to children
        {
            temp = delItem->next;
            free(delItem);
            delItem = temp;
        }
    }
    ht_init(table);// sets everything to NULL again
 }
