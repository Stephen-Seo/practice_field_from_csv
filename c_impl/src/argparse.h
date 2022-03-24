#ifndef SEODISPARATE_FIELDS_FROM_CSV_ARG_PARSE_H_
#define SEODISPARATE_FIELDS_FROM_CSV_ARG_PARSE_H_

#include "strdlist.h"

typedef struct ParsedArgs {
  const char *input_filename;
  StrDList *fields;
  int usage_print;
  int error;
} ParsedArgs;

ParsedArgs parse_args(int argc, char **argv);

#endif
// vim: et: ts=2: sts=2: sw=2
