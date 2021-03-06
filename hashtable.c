//#define NULL 0

#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* hash: form hash value for string s */
unsigned hash(char *s, int hash_size)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 31 * hashval;
    return hashval % hash_size;
}

/* Use 0 or a negative if you are not sure of the expected size. A hash table will regrow once too full. */
hashtable *create_hashtable(int expected_size)
{
    hashtable *ht;
    ht = malloc(sizeof(hashtable));
    if (ht == NULL)
        return NULL;
    if (expected_size <= 0)
        ht->size = DEFAULT_HASHSIZE;
    else
        ht->size = expected_size;

    ht->data = malloc(ht->size*sizeof(unsigned int*));
    if (ht->data == NULL)
        return NULL;
    memset(ht->data, 0, ht->size*sizeof(unsigned int*));
    return ht;
}

void destroy_hashtable(hashtable *ht)
{
    free(ht);
}

hashtable *regrow_hashtable(hashtable *ht, int new_hash_size)
{
    hashtable *new_ht;
    int i;
    int old_hash_size;

    new_ht = create_hashtable(new_hash_size);

    for (i=0; i<old_hash_size; i++) {
    }

    destroy_hashtable(ht);
    return(new_ht);
}
/* Main interface for 'lookup': look for s in hashtab */
struct nlist *lookup(hashtable *ht, char *s)
{
    struct nlist *np;
    for (np = ht->data[hash(s, ht->size)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
          return np; /* found */
    return NULL; /* not found */
}

/* Helper function: make a duplicate of s */
char *mystrdup(char *s)
{
    char *p;
    p = (char *) malloc(strlen(s)+1); /* +1 for ’\0’ */
    if (p != NULL)
       strcpy(p, s);
    return p;
}

/* Main interface for 'write_hashtable': put (name, defn) in hashtab */
struct nlist *write_hashtable(hashtable *ht, char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(ht, name)) == NULL) { /* not found */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
          return NULL;
        hashval = hash(name, ht->size);
        np->next = ht->data[hashval];
        ht->data[hashval] = np;
    } else /* already there */
        free((void *) np->defn); /*free previous defn */
    if ((np->defn = strdup(defn)) == NULL)
       return NULL;
    return np;
}
