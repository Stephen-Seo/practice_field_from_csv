#ifndef SEODISPARATE_FIELDS_FROM_CSV_STRDLIST_H_
#define SEODISPARATE_FIELDS_FROM_CSV_STRDLIST_H_

#include <stdlib.h>

typedef struct StrDList {
  char *string;
  struct StrDList *next;
  struct StrDList *prev;
} StrDList;

StrDList *create_strdlist();

void append_strdlist(StrDList *head, const char *string);

void cleanup_strdlist(StrDList **head);

size_t print_and_count_strdlist(StrDList *head);

int strdlist_has_str(StrDList *head, const char *str);

#endif
// vim: et: ts=2: sts=2: sw=2
