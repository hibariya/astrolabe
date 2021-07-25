#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wn.h"
#include "astrolabe.h"

static int poss[] = {
  NOUN,
  VERB,
  ADJ,
  ADV,
  SATELLITE
};

#define RESS(relation) relation->synset

static int cache_size = 100000;

int synset_cache_hash(unsigned long cache[cache_size], long value) {
  int hash = value % cache_size;

  return (hash + (3 - (hash % 3))) % cache_size;
}

int synset_cache_set(unsigned long cache[cache_size], int pos, long offset, Synset* synset) {
  int hash = synset_cache_hash(cache, offset + pos);

  int current = hash;
  for (int i = 0; i < (cache_size / 3); i++) {
    if (cache[current + 2] == 0 || (cache[current] == pos && cache[current + 1] == offset)) {
      cache[current] = pos;
      cache[current + 1] = offset;
      cache[current + 2] = (unsigned long)synset;
      return 1;
    }

    current = (current + 3) % cache_size;
  }

  return 0;
}

Synset* synset_cache_get(unsigned long cache[cache_size], int pos, long offset) {
  int hash = synset_cache_hash(cache, offset + pos);

  int current = hash;
  for (int i = 0; i < 50; i++) {
    int current_pos = current;
    int current_offset = current + 1;
    int current_synset = current + 2;

    if (cache[current_pos] == pos && cache[current_offset] == offset) {
      return (Synset*) cache[current_synset];
    }

    current = (current + 3) % cache_size;
  }

  return NULL;
}

Synset* fetch_synset(unsigned long cache[cache_size], int pos, long offset, char* word) {
  Synset* synset = synset_cache_get(cache, pos, offset);

  if (!synset) {
    synset = read_synset(pos, offset, word);
    synset_cache_set(cache, pos, offset, synset);
  } else {
  }

  return synset;
}

void free_synset_cache(unsigned long cache[cache_size]) {
  for (int i = 0; i < cache_size; i += 3) {
    if (cache[i] != 0 && cache[i + 1] != 0 && cache[i + 2] > 1) {
      free_synset((Synset*)cache[i + 2]);
    }
  }
}

Index* find_index(char* word, int pos) {
  Index* index = getindex(word, pos);

  if (index == NULL) {
    char* morphword;

    if (morphword = morphstr(word, pos)) {
      index = getindex(morphword, pos);
    }
  }

  return index;
}

Index* find_any_index(char* word) {
  Index* index;

  for (int i = 0; i < (sizeof(poss) / sizeof(poss[0])); i++) {
    index = find_index(word, poss[i]);

    if (index != NULL) return index;
  }

  return NULL;
}

int connect_first_match(SearchNode* head, char* word) {
  SearchNode* current = head;
  Relation* src = NULL;
  Relation* dst = NULL;

  while (1) {
    src = current->src;
    dst = current->dst;

    for (int i = 0; i < RESS(dst)->wcount; i++) {
      if (strcmp(RESS(dst)->words[i], word) == 0) {
        src->next = dst;
        return 1;
      }
    }

    if ((current = current->next) == NULL) break;
  }

  return 0;
}

SearchNode* make_next_search(SearchNode* node, unsigned long synset_cache[cache_size]) {
  SearchNode* next_node = NULL;
  SearchNode* current = node;
  while (1) {
    Relation* prev_src = current->src;
    Relation* next_src = current->dst;

    for (int i = 0; i < RESS(next_src)->ptrcount; i++) {
      Synset* dst_synset = fetch_synset(synset_cache, RESS(next_src)->ppos[i], RESS(next_src)->ptroff[i], "");
      SearchNode* tmp = malloc(sizeof(SearchNode));
      tmp->src = next_src;
      tmp->dst = malloc(sizeof(Relation));
      tmp->dst->synset = dst_synset;
      tmp->dst->type = RESS(next_src)->ptrtyp[i];
      tmp->dst->whichword = RESS(next_src)->pto[i];
      tmp->dst->next = NULL;
      tmp->next = NULL;
      if (next_node != NULL) tmp->next = next_node;
      next_node = tmp;
    }

    if ((current = current->next) == NULL) break;
  }

  return next_node;
}

void connect_first_hit(SearchNode* node, SearchNode* next_node) {
  SearchNode* current_next_node = next_node;
  while (1) {
    Relation* dst = current_next_node->src;

    if (dst->next != NULL) {
      SearchNode* current = node;
      while (1) {
        if (RESS(current->dst) == RESS(dst) && RESS(current->src) != RESS(dst)) {
          current->src->next = dst;
          break;
        }

        if ((current = current->next) == NULL) break;
      }

      break;
    }

    if ((current_next_node = current_next_node->next) == NULL) break;
  }
}

void free_search(SearchNode* node) {
  SearchNode* current = node;
  while (1) {
    SearchNode* previous = current;
    current = current->next;
    free(previous);

    if (current == NULL) break;
  }
}

void free_relation(Relation* relation) {
  Relation* current_relation = relation;
  while (1) {
    Relation* previous_relation = current_relation;
    current_relation = current_relation->next;

    free(previous_relation);
    if (current_relation == NULL) break;
  }
}

void search_path(SearchNode* node, char* word, int depth, unsigned long synset_cache[cache_size]) {
  if (depth > 10) depth = 10;
  if (connect_first_match(node, word)) return;
  if ((depth - 1) <= 0) return;

  SearchNode* next_node = make_next_search(node, synset_cache);
  if (next_node == NULL) return;

  search_path(next_node, word, depth - 1, synset_cache);
  connect_first_hit(node, next_node);

  free_search(next_node);
}
