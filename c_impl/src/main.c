#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StrDList {
  char *string;
  struct StrDList *next;
  struct StrDList *prev;
} StrDList;

typedef struct ParsedArgs {
  const char *input_filename;
  StrDList *fields;
  int usage_printed;
  int error;
} ParsedArgs;

void usage() {
  puts(
      "USAGE:\n"
      "./fields_from_csv <input_csv> <field> [<field>...]\n");
}

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

ParsedArgs parse_args(int argc, char **argv) {
  // init
  ParsedArgs args;
  args.input_filename = NULL;
  args.fields = create_strdlist();
  args.usage_printed = 0;
  args.error = 0;

  --argc;
  ++argv;

  // get filename and field from args
  if (argc > 1) {
    args.input_filename = argv[0];
  } else {
    args.error = 1;
    usage();
    args.usage_printed = 1;
  }

  --argc;
  ++argv;

  while (argc > 0) {
    append_strdlist(args.fields, argv[0]);
    --argc;
    ++argv;
  }

  return args;
}

typedef struct CSVEntry {
  char *buf;
  size_t idx;
  int is_end_of_line;
  int is_end_of_file;
  int is_chosen_field;
} CSVEntry;

CSVEntry get_field(FILE *fd, size_t field_idx) {
  size_t current_size = 512;
  CSVEntry entry;
  entry.is_end_of_line = 0;
  entry.is_end_of_file = 0;
  entry.is_chosen_field = 0;
  entry.buf = NULL;
  size_t idx = 0;
  int next = fgetc(fd);

  while (next != EOF) {
    if (next == ',') {
      break;
    } else if (next == '\n') {
      entry.is_end_of_line = 1;
      break;
    } else if (next == ' ' || next == '\t') {
      next = fgetc(fd);
      continue;
    }
    if (!entry.buf) {
      // malloc of buf deferred to here in case a field reaches EOF
      entry.buf = malloc(current_size);
    }
    entry.buf[idx++] = next;
    if (idx >= current_size) {
      current_size *= 2;
      entry.buf = realloc(entry.buf, current_size);
    }

    next = fgetc(fd);
  }

  if (next == EOF) {
    entry.is_end_of_file = 1;
  }

  if (entry.buf) {
    if (idx >= current_size) {
      current_size += 1;
      entry.buf = realloc(entry.buf, current_size);
      entry.buf[idx] = 0;
    } else {
      entry.buf[idx] = 0;
    }
  }

  entry.idx = field_idx;

  return entry;
}

int main(int argc, char **argv) {
  ParsedArgs args = parse_args(argc, argv);

  if (args.error) {
    cleanup_strdlist(&args.fields);
    return 1;
  } else if (args.usage_printed) {
    cleanup_strdlist(&args.fields);
    return 0;
  }

  print_and_count_strdlist(args.fields);

  FILE *csv_fd = fopen(args.input_filename, "r");
  if (!csv_fd) {
    printf("ERROR: Failed to open %s!\n", args.input_filename);
    return 2;
  }

  size_t cols_size = 32;
  CSVEntry *entries = calloc(cols_size, sizeof(CSVEntry));

  // get first row of fields and mark the picked ones out of them
  size_t idx = 0;
  while (1) {
    entries[idx] = get_field(csv_fd, idx);
    ++idx;

    if (idx >= cols_size) {
      // dynamically resize if more space is needed
      cols_size *= 2;
      entries = realloc(entries, cols_size * sizeof(CSVEntry));
      memset(entries + (cols_size / 2), 0, cols_size / 2);
    }

    // mark entry if it is specified entry
    if (strdlist_has_str(args.fields, entries[idx - 1].buf) >= 0) {
      printf("Found chosen field at idx %ld\n", idx - 1);
      entries[idx - 1].is_chosen_field = 1;
    }

    // break if end of first line is reached
    // TODO maybe check for the case where there is a newline immediately
    // after a comma
    if (entries[idx - 1].is_end_of_line) {
      break;
    }
  }

  // do printing of only the chosen columns
  size_t number_of_entries = idx;
  idx = 0;
  while (1) {
    CSVEntry entry = get_field(csv_fd, idx);
    if (entries[idx].is_chosen_field && entry.buf) {
      printf("%s, ", entry.buf);
    }

    if (entry.buf) {
      free(entry.buf);
    }

    if (entry.is_end_of_file) {
      break;
    } else if (entry.is_end_of_line) {
      idx = 0;
      printf("\n");
    } else {
      ++idx;
    }
  }

  fclose(csv_fd);
  for (size_t i = 0; i < number_of_entries; ++i) {
    if (entries[i].buf) {
      free(entries[i].buf);
    }
  }
  free(entries);
  cleanup_strdlist(&args.fields);
  return 0;
}
