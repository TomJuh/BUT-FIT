/**
 * Kostra pro implementaci mnozinovych operaci.
 * Predmet IZP 2020/21
 * @Author Ales Smrcka
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
 
/**
 * items bude ukazovat na alokovanou pamet, pouze pokud mnozina obsahuje nejaky
 * prvek
 * card vyjadruje pocet prvku v mnozine
 */
typedef struct {
    int *items;
    int card;
//    int capacity; // volitelny atribut, vyzaduje upravit vyznam items a card
} set_t;
 
/* ****** LADICI POMUCKY, NEMENTE ****************** */
void debug_set(const set_t s, const char *name)
{
    printf("Set %s = { .card = %d, .items(%p) = {", name, s.card, s.items);
    for (int i = 0; i < s.card; i++)
        printf(i ? ",%d" : "%d", s.items[i]);
    puts("} }");
}
#define r(cond, msg, s) ((cond) ? fprintf(stdout, "%s ok\n", msg) : (fprintf(stderr, "%s failed: \"%s\", ", msg, #cond),debug_set(s, #s),exit(1),0))
 
 
/* ****** DOPLNTE KOD NASLEDUJICICH FUNKCI ****** */
 
// Inicializuje mnozinu na prazdnou mnozinu
void set_init(set_t *s)
{
    s->items=NULL;
    s->card=0;
}
 
// Vraci true, pokud je v mnozine s prvek x
bool set_contains(const set_t *s, int x)
{
    for (int i=0;i<s->card;i++){
        if(s->items[i]==x){
            return true;
        }

    }
    return false; 
} 
 
// Vraci true, pokud je mnozina s prazdna
bool set_is_empty(const set_t *s){
    if (s->items==NULL){
        return true;
    }
    return false;     
}
 
// Prida prvek x do mnoziny s
void set_add_item(set_t *s, int x)
{
    if (!(s->card>=0)){
       s->card = 0;
    }
    if (!set_contains(s,x)){
         s->items = realloc(s->items,sizeof(int)*(s->card+1));
         if (s->items!=NULL){
         s->items[s->card] = x;
         s->card++;
         }else{
         free(s->items);}
        assert(s->items!=NULL);
    }
}
 
// Odstrani z mnoziny s prvek x
void set_remove_item(set_t *s, int x)
{
    for (int i=0;i<s->card;i++){
        if (s->items[i]==x){
            for (int l=i;l<s->card;l++){
              
                s->items[l] = s->items[l+1]; 
            }
            s->card--;

            break;
        }

    }
}
 
// Prida vsechny prvky z pole src o velikosti n do mnoziny s
void set_copy_items(set_t *s, const int src[], int n)
{
    if (n>0){
    for (int i=0;i<n;i++){
        set_add_item(s,src[i]);
        }
    }
}
 
// Odstrani vsechny prvku z mnoziny
void set_clear(set_t *s)
{
    free(s->items);
    s->items = NULL;
    s->card = 0;
}
 
// Vytvori kopii mnoziny src do mnoziny dst. Mnozina dst muze byt neinicializovana.
void set_copy(set_t *dst, const set_t *src)
{
    if (dst->card > 0){
     for (int i=0;i<src->card;i++){
        set_add_item(dst,src->items[i]); 

     }
    }else{
        set_init(dst);


        for (int i=0;i<src->card;i++){
        set_add_item(dst,src->items[i]);
        }  
    }
    
    
}
 
// Do mnoziny s ulozi sjednoceni mnozin a a b
void set_union(set_t *s, const set_t *a, const set_t *b)
{
    for (int i=0;i<a->card;i++){
        if (!set_contains(s,a->items[i])){
                set_add_item(s,a->items[i]);
            }

    }
    for (int i=0;i<b->card;i++){
        if (!set_contains(s,b->items[i])){
                set_add_item(s,b->items[i]);
            }

    }
}
 
// Do mnoziny s ulozi prunik mnozin a a b
void set_intersection(set_t *s, const set_t *a, const set_t *b)
{
    for (int i=0;i<a->card;i++){
            if (set_contains(b,a->items[i])){
                set_add_item(s,a->items[i]);
            }

    }
}
 
// Do mnoziny s ulozi rozdil mnozin a \ b
void set_minus(set_t *s, const set_t *a, const set_t *b)
{
    
    if (s->items==a->items){
        for (int i=0;i<b->card;i++){
            if(set_contains(a,b->items[i])){
                set_remove_item(s,b->items[i]);
            }
        }
    }else{
        for (int i=0;i<a->card;i++){
            if(!set_contains(b,a->items[i])){
                set_add_item(s,a->items[i]);
            }
        
        }
    }
}
 
/* ****** NASLEDUJICI KOD NEMENTE ****** */
int main()
{
    set_t a;
 
    // inicializace
    set_init(&a);
 
    r(a.card == 0, "set_init", a);
 
    r(set_is_empty(&a), "set empty", a);
 
    // mnozina by nemela obsahovat prvek
    r(!set_contains(&a, 42), "contains 42", a);
 
    // pridani prvku do mnoziny
    set_add_item(&a, 42);
    r(a.card == 1, "add item 42", a);
    r(!set_is_empty(&a), "set nonempty", a);
 
       // prvek by mel byt v mnozine
    r(set_contains(&a, 42), "contains 42", a);
 
    // pridani stejneho prvku do mnoziny
    set_add_item(&a, 42);
    r(a.card == 1, "add same item 42", a);
 
    set_add_item(&a, 1);
    r(a.card == 2, "add item 1", a);
 
    set_remove_item(&a, 1);
    r(a.card == 1, "remove 1", a);
    set_remove_item(&a, 2);
    r(a.card == 1, "remove 2 (non-included item)", a);
 
    set_clear(&a);
    r(a.card == 0, "clear set", a);
 
    int arr1[] = {1,3,4,5};
    int arr2[] = {1,2,3};
 
    set_t x, y;
    
    set_init(&x);
    set_copy_items(&x, arr1, 4);
    r(x.card == 4, "copy set x", x);
    r(set_contains(&x, 5), "copy set, contains item 5", x);
 
    set_init(&y);
    set_copy_items(&y, arr2, 3);
    r(y.card == 3, "copy set y", y);
 
    set_union(&a, &x, &y);
    r(a.card == 5, "union: a <- x CUP y", a);
    r(set_contains(&a, 4), "1 in a", a);
    r(set_contains(&a, 2), "2 in a", a);
    set_clear(&a);

    set_intersection(&a, &x, &y);
    r(a.card == 2, "intersection: a <- x CAP y", a);
    r(set_contains(&a, 1), "1 in a", a);
    r(!set_contains(&a, 2), "2 not in a", a);
    set_clear(&a);
 
    set_minus(&a, &y, &x);
    r(a.card == 1, "minus: a <- {1,2,3} \\ {1,3,4,5}", a);
    r(set_contains(&a, 2), "2 in a", a);
    set_clear(&a);
    set_copy(&a, &x);
    r(a.card == 4, "copy set a <- x", a);
    set_remove_item(&x, 4);
    r(set_contains(&a, 4), "deep copy a <- x", a);
 
    // perlicka na zaver
    set_minus(&a, &a, &y);
    r(a.card == 2, "minus: a <- a({1,3,4,5}) \\ {1,2,3}", a);
    r(set_contains(&a, 4), "4 in a", a);
    r(set_contains(&a, 5), "5 in a", a);
 
    set_clear(&a);
    set_clear(&x);
    set_clear(&y);
 
    return 0;
}
