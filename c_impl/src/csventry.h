#ifndef SEODISPARATE_FIELDS_FROM_CSV_CSVENTRY_H_
#define SEODISPARATE_FIELDS_FROM_CSV_CSVENTRY_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct CSVEntry {
  char *buf;
  size_t idx;
  int is_end_of_line;
  int is_end_of_file;
  int is_chosen_field;
} CSVEntry;

CSVEntry get_field(FILE *fd, size_t field_idx);
void cleanup_csventry(CSVEntry *csvEntry, int nullify);

#endif
// vim: et: ts=2: sts=2: sw=2
