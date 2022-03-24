#include "strdlist.h"

#include <stdio.h>
#include <string.h>

StrDList *create_strdlist() { return calloc(1, sizeof(StrDList)); }

void append_strdlist(StrDList *head, const char *string) {
  StrDList *current = head;

  while (current->next) {
    current = current->next;
  }

  current->next = calloc(1, sizeof(StrDList));
  current->next->prev = current;
  size_t string_size = strlen(string);
  current->next->string = malloc(string_size + 1);
  memcpy(current->next->string, string, string_size);
  current->next->string[string_size] = 0;
}

void cleanup_strdlist(StrDList **head) {
  if (!head || !*head) {
    return;
  }

  StrDList *current = *head;
  while (current->next) {
    current = current->next;
  }

  while (current->prev) {
    current = current->prev;
    if (current->next->string) {
      free(current->next->string);
    }
    free(current->next);
    current->next = NULL;
  }

  (*head)->next = NULL;

  free(*head);
  *head = NULL;
}

size_t print_and_count_strdlist(StrDList *head) {
  size_t count = 0;
  StrDList *current = head;
  puts("Printing strdlist...");
  while (current->next) {
    current = current->next;
    printf("%s ", current->string);
    ++count;
  }
  printf("\n");

  return count;
}

int strdlist_has_str(StrDList *head, const char *str) {
  StrDList *current = head;
  int idx = 0;
  while (current->next) {
    current = current->next;
    if (strcmp(current->string, str) == 0) {
      return idx;
    } else {
      ++idx;
    }
  }

  return -1;
}

// vim: et: ts=2: sts=2: sw=2
