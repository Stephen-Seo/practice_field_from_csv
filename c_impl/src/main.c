#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"
#include "csventry.h"
#include "strdlist.h"

void usage() {
  puts(
      "USAGE:\n"
      "./fields_from_csv <input_csv> <field> [<field>...]\n");
}

int main(int argc, char **argv) {
  ParsedArgs args = parse_args(argc, argv);

  if (args.error) {
    cleanup_strdlist(&args.fields);
    return 1;
  } else if (args.usage_print) {
    usage();
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

    cleanup_csventry(&entry, 1);

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
    cleanup_csventry(entries + i, 0);
  }
  free(entries);
  cleanup_strdlist(&args.fields);
  return 0;
}

// vim: et: ts=2: sts=2: sw=2
