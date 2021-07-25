#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wn.h"
#include "astrolabe.h"

static char* search_type_labels[] = {
  "(unknown)",
  "Antonym",
  "Hypernym",
  "Hyponym",
  "Entailment",
  "Similar",
  "Member meronym",
  "Substance meronym",
  "Part meronym",
  "Member holonym",
  "Substance holonym",
  "Part holonym",
  "Meronym",
  "Holonym",
  "Cause",
  "Participle of verb",
  "Also see",
  "Pertains to noun or derived from adjective",
  "Attribute",
  "Verb group",
  "Derivationally related form",
  "Domain of synset",
  "Member of this domain",
  "Synonym",
  "Polysemy",
  "Verb example sentences and generic frames",
  "Noun coordinate",
  "Group related sense",
  "Hierarchical meronym",
  "Hierarchical holonym",
  "Keywords by substring",
  "Overview"
  "Domain topic",
  "Domain usage",
  "Domain Region",
  "Domain term for topic",
  "Domain term for usage",
  "Domain term for region",
  "Instance of",
  "Instance"
};

static char* pos_labels[] = {
  "(unknown)",
  "Noun",
  "Verb",
  "Adjective",
  "Adverb",
  "Satellite"
};

static int poss[] = {
  NOUN,
  VERB,
  ADJ,
  ADV,
  SATELLITE
};

#define RESS(relation) relation->synset

static int cache_size = 100000;

void bold_start() { printf("\x1b[;1m"); }
void bold_end() { printf("\x1b[0m"); }

void draw_path(Relation* relation, char* word, char* other_word) {
  Relation* current = relation;
  int depth = 0;
  while(1) {
    for (int i = 0; i < depth; i++) { printf("    "); }

    if (depth > 0) {
      bold_start();
      printf("-- %s --> ", search_type_labels[current->type]);
      bold_end();
    }

    printf("Sense#%ld: ", RESS(current)->hereiam);

    for (int j = 0; j < RESS(current)->wcount; j++) {
      if (!strcmp(RESS(current)->words[j], word) || !strcmp(RESS(current)->words[j], other_word)) {
        bold_start();
        printf("%s#%d;", RESS(current)->words[j], RESS(current)->wnsns[j]);
        bold_end();
      } else {
        printf("%s#%d;", RESS(current)->words[j], RESS(current)->wnsns[j]);
      }
    };

    printf(" (%s)", pos_labels[getpos(RESS(current)->pos)]);
    puts("");

    for (int i = 0; i < depth; i++) { printf("    "); }
    printf("%s\n", RESS(current)->defn);

    if (current->next == NULL) break;

    current = current->next;
    depth++;
  }
}

int main(int argc, char *argv[]) {
  wninit();

  char* word = argv[1];
  char* other_word = argv[2];

  Index* other_index = find_any_index(other_word);
  if (other_index == NULL) return 1;

  unsigned long synset_cache[cache_size];
  memset(synset_cache, 0, sizeof(synset_cache));

  printf("From: \"%s\", To: \"%s\"\n", word, other_index->wd);

  for (int i = 0; i < (sizeof(poss) / sizeof(poss[0])); i++) {
    int pos = poss[i];
    char* pos_label = pos_labels[pos];

    Index* index = find_index(word, pos);
    if (index == NULL) continue;

    printf("Finding \"%s\" (%s)\n", index->wd, pos_label);

    for (int j = 0; j < index->off_cnt; j++) {
      Synset* synset = fetch_synset(synset_cache, pos, index->offset[j], index->wd);
      if (synset == NULL) continue;

      Relation* relation = malloc(sizeof(Relation));
      relation->synset = synset;
      relation->type = 0;
      relation->whichword = synset->whichword;
      relation->next = NULL;
      SearchNode node = {.src = relation, .dst = relation, .next = NULL};
      search_path(&node, other_index->wd, 5, synset_cache);

      if (relation->next == NULL) continue;
      if (RESS(relation)->hereiam == RESS(relation->next)->hereiam) {
        relation->next = NULL;
      }

      draw_path(relation, word, other_word);
      puts("");

      free_relation(relation);
    }

    free_index(index);
  }

  free_index(other_index);
  free_synset_cache(synset_cache);

  return 0;
}
