//===-- options.cpp - Program options interface ---------------------------===//
//
// Copyright (c) 2023 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; a full copy
// of the license can be found in the LICENSE.txt file.
//
//===----------------------------------------------------------------------===//

#include "options.h"

#include <iostream>

#include <unistd.h>

constexpr auto usage =
    "Usage: byrep [-h] [-i | -o <path>] [-s <sub> ...] <file>\n";

constexpr auto full_usage = R"(
Arguments:
  file              Input file path

Options:
  -s <sub>          Byte pattern substitution to perform (see notes below)
  -o <path>         Output file path (if not using `-i`)

Flags:
  -i                Patch the input file in-place (equivalent to `-o <file>`)
  -h                Show this help and usage message

Notes:
  Substitutions are formatted as two hexadecimal byte patterns joined with a
  forward slash ('/') character, i.e. `<old>/<new>`, e.g. `6a70/4a50`.

  The `-s` parameter can be repeated multiple times to perform multiple
  substitutions.
)";

void Options::show_usage_and_exit(bool was_requested) {
  std::cerr << usage;
  if (was_requested)
    std::cerr << full_usage;

  std::exit(was_requested ? 0 : 1);
}

Options::Options(int argc, char **argv)
    : wants_help(false), wants_in_place(false) {
  int opt_char = 0;
  while ((opt_char = getopt(argc, argv, "hio:s:")) != -1) {
    switch (opt_char) {
    case '?':
    case 'h':
      show_usage_and_exit(true);
      break;
    case 'i':
      if (!output_path.empty()) {
        std::cerr << "Error: Cannot use `-i` with `-o`.\n";
        show_usage_and_exit(false);
      }
      wants_in_place = true;
      break;
    case 'o':
      if (wants_in_place) {
        std::cerr << "Error: Cannot use `-o` with `-i`.\n";
        show_usage_and_exit(false);
      }
      output_path = optarg;
      break;
    case 's':
      substitutions.emplace_back(optarg);
      break;
    default:
      show_usage_and_exit(false);
      break;
    }
  }

  if (optind >= argc) {
    std::cerr << "Error: Missing input file.\n";
    show_usage_and_exit(false);
  }
  input_path = argv[optind];

  if (!wants_in_place && output_path.empty()) {
    std::cerr << "Error: Must specify output path (`-o`) or in-place "
                 "patching (`-i`).\n";
    show_usage_and_exit(false);
  }
}
