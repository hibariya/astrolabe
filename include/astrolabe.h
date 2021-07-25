#include "wn.h"

#define ASTROLABE_CACHE_SIZE 100000

typedef struct relation {
  Synset* synset;
  int type;
  int whichword;
  struct relation* next;
} Relation;

typedef struct search_node {
  Relation* src;
  Relation* dst;
  struct search_node* next;
} SearchNode;

extern int synset_cache_hash(unsigned long*, long);
extern int synset_cache_set(unsigned long*, int, long, Synset*);
extern Synset* synset_cache_get(unsigned long*, int, long);
extern void free_synset_cache(unsigned long*);
extern Synset* fetch_synset(unsigned long*, int, long, char*);

extern Index* find_index(char*, int);
extern Index* find_any_index(char*);

extern void free_relation(Relation*);

extern void search_path(SearchNode*, char*, int, unsigned long*);
