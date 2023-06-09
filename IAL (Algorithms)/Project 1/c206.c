/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    list->activeElement = NULL; //inicializes elements by setting them to null
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    while (list->lastElement!= NULL){ //goes from last element to first
        list->activeElement = list->lastElement; //uses activeElement as tempElement for switching
        list->lastElement= list->lastElement->previousElement; //makes the new last element the second to last element
        free(list->activeElement); //frees the last element
    }
    list->activeElement = NULL; //sets all elements to NULL
    list->firstElement = NULL;
    list->lastElement = NULL;

}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    DLLElementPtr  newElem =  malloc (sizeof(struct DLLElement)); //allocates new element
    if (!newElem){ //checks malloc
        DLL_Error();
        return;
    }
    newElem->data = data; //sets data 
    newElem->nextElement = list->firstElement; //makes the first element second to the new one
    newElem->previousElement = NULL; //there is nothing before newElem
    if (list->firstElement != NULL) //if firstElem was defined
        list->firstElement->previousElement= newElem; //newElem will be left of (behind) newElem
    else
        list->lastElement = newElem; //if it wasn't defined newElem is also the lastOne
    list->firstElement = newElem; //sets newElem as firstElem
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    DLLElementPtr  newElem =  malloc (sizeof(struct DLLElement)); //allocates new Elem
    if (!newElem){ //malloc check
        DLL_Error();
        return;
    }
    newElem->data = data; //sets data
    newElem->nextElement = NULL; //there is nothing after newElem
    newElem->previousElement = list->lastElement; //sets lastElem as previous of newElem
    
    if (list->lastElement != NULL) //if lastElem was defined
        list->lastElement->nextElement= newElem; //makes next elem of lastElem the newElem
    else
        list->firstElement = newElem; //otherwise makes newElem the first one as well
    list->lastElement = newElem; //sets new elem as lastElem

}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) { //makes the firstElement the active one
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) { //makes the lastElement the active one
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) { //if firstElement exist return it through a pointer
    if (list->firstElement == NULL)
        DLL_Error();
    else 
        *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) { //if lastElement exist return it through a pointer
    if (list->lastElement == NULL)
        DLL_Error();
    else
        *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    DLLElementPtr  elemPtr; //temp Elem
    if (list->firstElement != NULL){ //if last elem is defined
        elemPtr = list->firstElement; //sets it to firstElem
        if (list->activeElement == list->firstElement) //deactivates first elem
            list->activeElement = NULL;
        if (list->firstElement == list->lastElement) //if firstElem is also the last one both get removed
            list->firstElement = list->lastElement= NULL;
        else {
            list->firstElement = list->firstElement->nextElement; //set the firstElem to the second elem
            list->firstElement->previousElement = NULL; //sets previous to NULL
        } 
        free(elemPtr); //frees temp elem
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    DLLElementPtr  elemPtr; //tempElem
    if (list->lastElement != NULL){ //if lastElem was defined
        elemPtr = list->lastElement; //sets elem as lastElem
        if (list->activeElement == list->lastElement)//deactivates elem
            list->activeElement = NULL;
        if (list->firstElement == list->lastElement) //if firstElem is also the last one both get removed
            list->firstElement = list->lastElement= NULL;
        else {
            list->lastElement = list->lastElement->previousElement; //sets lastElem as the second to last
            list->lastElement->nextElement = NULL; //sets nextElem of lastElem as NULL
        } 
        free(elemPtr);//frees temp elem
    }}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    if (list->activeElement != NULL && list->activeElement->nextElement != NULL) { //if elem is active and has nextElem
        DLLElementPtr  elemPtr= list->activeElement->nextElement; //tempElem set to the nextElem of activeElem
        list->activeElement->nextElement = elemPtr->nextElement; //sets actives nextElem to the nextElem of itself 
        if (elemPtr == list->lastElement) //if tempElem is the lastOne makes it active
            list->lastElement = list->activeElement;
        else
            elemPtr->nextElement->previousElement = list->activeElement; //makes tempElem point to activeElem
        free(elemPtr); //frees tempElem
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    if (list->activeElement != NULL && list->activeElement->previousElement != NULL){//if there is activeElem and elem before it
        DLLElementPtr elemPtr = list->activeElement->previousElement; //tempElem set to previous of active
        list->activeElement->previousElement = elemPtr->previousElement;//sets actives previosElem to the previousElem of itself
        if (elemPtr == list->firstElement) //if tempElem is the firstOne makes it active
            list->firstElement = list->activeElement;
        else 
            elemPtr->previousElement->nextElement = list->activeElement; //makes tempElem point to activeElem
        free(elemPtr); //frees tempElem
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
    if (list->activeElement != NULL){ //if any elem in list is active
        DLLElementPtr newElem = malloc (sizeof(struct DLLElement)); //allocates new elem
        if (newElem == NULL){ //malloc check
            free(newElem);
            return;
        }
        newElem->data = data; //sets data
        newElem->nextElement = list->activeElement->nextElement; //sets nextElem to active's nextElem
        newElem->previousElement = list->activeElement; //makes activeElem the previous of newElem
        list->activeElement->nextElement = newElem;

        if (list->activeElement == list->lastElement) //if active is lastElem
            list->lastElement = newElem; //makes newElem the lastElem
        else 
            newElem->nextElement->previousElement = newElem; //sets itself as left neighbor of it's nextElem 
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    if (list->activeElement != NULL){ //if any elem in list is active
        DLLElementPtr newElem = malloc (sizeof(struct DLLElement)); //allocates newElem
        if (newElem == NULL){ //mallock check
            free(newElem);
            return;
        }
        newElem->data = data; //sets data
        newElem->previousElement = list->activeElement->previousElement; //sets newElem's previousElem to active's previousElem
        newElem->nextElement = list->activeElement; //sets nextElem of newElem to activeElem
        list->activeElement->previousElement = newElem; //sets active's previousElem to new Elem

        if (list->activeElement == list->firstElement) //if activeElem is the firstElem
            list->firstElement = newElem; //firstElem is set to newElem
        else 
            newElem->previousElement->nextElement= newElem; //sets itself as a right neighbor of it's previousElem
    }}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) { //if list is active return value through a pointer
    if (list->activeElement == NULL)
        DLL_Error();
    else
        *dataPtr = list->activeElement->data; 
    }

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) { //if list is active sets data
    if (list->activeElement !=  NULL)
        list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) { //if list is active set it to next element
    if (list->activeElement != NULL)
        list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) { //if list is active set it to previous element
    if (list->activeElement != NULL)
        list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) { //if active isnt null list is active
    return list->activeElement != NULL;
}

/* Konec c206.c */
