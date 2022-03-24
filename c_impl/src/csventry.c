#include "csventry.h"

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

void cleanup_csventry(CSVEntry *csvEntry, int nullify) {
  if (csvEntry->buf) {
    free(csvEntry->buf);
  }
  if (nullify) {
    csvEntry->buf = NULL;
  }
}

// vim: et: ts=2: sts=2: sw=2
