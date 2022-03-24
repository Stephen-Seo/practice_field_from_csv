#include "argparse.h"

ParsedArgs parse_args(int argc, char **argv) {
  // init
  ParsedArgs args;
  args.input_filename = NULL;
  args.fields = create_strdlist();
  args.usage_print = 0;
  args.error = 0;

  --argc;
  ++argv;

  // get filename and field from args
  if (argc > 1) {
    args.input_filename = argv[0];
  } else {
    args.error = 1;
    args.usage_print = 1;
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
// vim: et: ts=2: sts=2: sw=2
