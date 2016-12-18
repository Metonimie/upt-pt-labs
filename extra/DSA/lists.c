#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct candidate_l_t {
  char name[30];
  int age       : 8;
  int convicted : 1;
  struct candidate_l_t * next_candidate;
};

struct party_l_t {
  char name[30];
  char symbol[10];
  struct candidate_l_t * candidates;
  struct party_l_t     * next_party;
};

typedef struct candidate_l_t * Member;
typedef struct party_l_t     * Party;

void delete_candidate(Party head, const char * name);
/* Functions for adding a party or candidate */

void add_party(Party head, Party party) {
  while (head->next_party != NULL) {
    head = head->next_party;
  }
  head->next_party = party;
}

void add_candidate(Member head, Member candidate) {
  while (head->next_candidate != NULL) {
    head = head->next_candidate;
  }
  head->next_candidate = candidate;
}

void add_candidate_to_party(Party party, Member candidate) {
  if ( party->candidates == NULL) {
    party->candidates = candidate;
  } else {
    add_candidate(party->candidates, candidate);
  }
}

struct party_l_t * new_party(const char * name, const char * symbol) {
  struct party_l_t * party = calloc(1, sizeof(struct party_l_t));
  strncpy(party->name, name, 30);
  strncpy(party->symbol, symbol, 10);
  return party;
}

struct candidate_l_t * new_candidate(const char * name, int age,
                                        int convicted) {
  struct candidate_l_t * candidate = calloc(1, sizeof(struct candidate_l_t));
  strncpy(candidate->name, name, 30);
  candidate->age = (age > 255) ? -1 : age;
  candidate->convicted = (convicted >= 1) ? 1 : 0;
  return candidate;
}

/* Functions for searching party and candidate list */

struct party_l_t *  search_party(Party head, const char * name) {
  while (head != NULL) {
    if ( strcmp(head->name, name) == 0) {
      return head;
    }
    head = head->next_party;
  }
  return NULL;
}

struct party_l_t * search_candidate(Party head, const char * name) {
  Member candidates = NULL;
  while (head != NULL) {
    candidates = head->candidates;

    while (candidates != NULL) {
      if ( strcmp(candidates->name, name) == 0 ) {
        return head;
      }
      candidates = candidates->next_candidate;
    }

    head = head->next_party;
  }
  return NULL;
}

struct candidate_l_t * get_canditate(Party head, const char * name) {
  Party current_party = search_candidate(head, name);
  Member candidates = current_party->candidates;

  while (candidates != NULL) {
    if ( strcmp(candidates->name, name) == 0 ) {
      return candidates;
    }
    candidates = candidates->next_candidate;
  }
  return NULL;
}

/* Functions for modifying party and candidate attributes */

void modify_candidate(Member candidate, const char * name, int age,
                      int convicted) {
  strncpy(candidate->name, name, 30);
  candidate->age = (age > 255) ? -1 : age;
  candidate->convicted = (convicted >= 1) ? 1 : 0;
}

void modify_party(Party party, const char * name, const char * symbol) {
  strncpy(party->name, name, 30);
  strncpy(party->symbol, symbol, 10);
}

void move_candidate(Party head, Party new_party, const char * name) {
  Party current_party = search_candidate(head, name);
  Member candidate = get_canditate(current_party, name);
  Member copy = new_candidate(candidate->name, candidate->age,
                              candidate->convicted);
  delete_candidate(current_party, name);
  add_candidate_to_party(new_party, copy);
}

/* Functions for deleting parties and candidates */

void delete_candidate(Party head, const char * name) {
  Party candidate_party = search_candidate(head, name);
  Member member_list = NULL;
  if (candidate_party != NULL) {
    member_list = candidate_party->candidates;
    if ( strcmp(member_list->name, name) == 0 ) {
      Member temp = candidate_party->candidates;
      candidate_party->candidates = candidate_party->candidates->next_candidate;
      free(temp);
      return;
    }
    while ( member_list != NULL ) {
      if ( strcmp(member_list->next_candidate->name, name) == 0) {
        Member temp = member_list->next_candidate;
        member_list->next_candidate = member_list->next_candidate->next_candidate;
        free(temp);
        return;
      }
      member_list = member_list->next_candidate;
    }
  }
}

void delete_candidate_list(Party head) {
  if (head == NULL) {
    return;
  }
  Member temp = NULL;
  while ( head->candidates != NULL) {
    temp = head->candidates;
    head->candidates = head->candidates->next_candidate;
    free(temp);
  }
}

struct party_l_t * delete_party(Party head, const char * name) {
  Party temp = search_party(head, name);
  delete_candidate_list(temp);
  Party * aux = &head;
  while (*aux) {
    if ( strcmp((*aux)->name, name) == 0 ) {;
      Party temp2 = (*aux)->next_party;
      free(*aux);
      *aux = temp2;
      return head;
    }
    aux = &(*aux)->next_party;
  }
  return NULL;
}

/* Function for printing all the party members.*/

void print_all(Party head) {
  printf("\n");
  Member member_list = NULL;
  while(head != NULL) {
    printf("Party Name: %s; Party Symbol: %s\n", head->name, head->symbol);
    member_list = head->candidates;
    while ( member_list != NULL ) {
      printf("\tName: %s; Age: %d; Convicted: %s\n", member_list->name,
      member_list->age, (member_list->convicted) ? "yes" : "no");
      member_list = member_list->next_candidate;
    }
    member_list = NULL;
    head = head->next_party;
  }
  printf("\n");
}

int main(void) {
  Party head = new_party("House Stark", "Wolf");
  Member bran = new_candidate("bran", 12, 0);
  Member ed = new_candidate("Ed", 40, 1);
  Member sansa = new_candidate("sansa", 14, 0);

  Party lannister = new_party("House Lannister", "Tiger");
  Member jam = new_candidate("Jamie", 30, 1);
  Member tyr = new_candidate("Tyrion", 41, 0);
  Member cer = new_candidate("Cersei", 30, 1);

  Party targaryan = new_party("House Targaryen", "Dragon");
  Member varis = new_candidate("varis", 40, 0);
  add_candidate_to_party(targaryan, varis);

  add_party(head, lannister);
  add_party(head, targaryan);
  add_candidate_to_party(head, bran);
  add_candidate_to_party(head, ed);
  add_candidate_to_party(head, sansa);
  add_candidate_to_party(lannister, jam);
  add_candidate_to_party(lannister, tyr);
  add_candidate_to_party(lannister, cer);

  // delete_candidate(head, "bran");
  // delete_candidate(head, "Tyrion");
  // delete_candidate_list(head);
  move_candidate(head, targaryan, "Ed");

  print_all(head);
  print_all(head);
  return EXIT_SUCCESS;
}
