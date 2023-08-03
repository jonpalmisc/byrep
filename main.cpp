//===-- main.cpp - Binary find & replace ----------------------------------===//
//
// Copyright (c) 2023 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; a full copy
// of the license can be found in the LICENSE.txt file.
//
//===----------------------------------------------------------------------===//

#include "buffer.h"
#include "encoding.h"
#include "options.h"
#include "timer.h"

#include <chrono>
#include <iostream>

using Substitution = std::pair<Bytes, Bytes>;

enum class SubstitutionParseError {
  Ok,
  MissingSeparator,
  BadPattern,
  BadReplacement
};

SubstitutionParseError parse_substitution(std::string const &literal,
                                          Substitution &result) {
  auto sep_index = literal.find('/');
  if (sep_index == std::string::npos)
    return SubstitutionParseError::MissingSeparator;

  auto pattern_hex = literal.substr(0, sep_index);
  result.first = Hex::decode(pattern_hex);
  if (result.first.empty())
    return SubstitutionParseError::BadPattern;

  auto replacement_hex =
      literal.substr(sep_index + 1, literal.size() - sep_index);
  result.second = Hex::decode(replacement_hex);
  if (result.second.empty())
    return SubstitutionParseError::BadReplacement;

  return SubstitutionParseError::Ok;
}

int main(int argc, char **argv) {
  Options options(argc, argv);
  ScopedTimer::set_logging_enabled(options.wants_verbose_output);

  std::vector<Substitution> substitutions(options.substitutions.size());
  for (size_t i = 0; i < substitutions.size(); ++i) {
    switch (parse_substitution(options.substitutions[i], substitutions[i])) {
    case SubstitutionParseError::MissingSeparator:
      std::cerr << "Error: Failed to parse substitution " << i
                << "; missing separator.\n";
      return 1;
    case SubstitutionParseError::BadPattern:
      std::cerr << "Error: Failed to parse substitution " << i
                << "; failed to hex-decode pattern.\n";
      return 1;
    case SubstitutionParseError::BadReplacement:
      std::cerr << "Error: Failed to parse substitution " << i
                << "; failed to hex-decode replacement.\n";
      return 1;
    default:
      break;
    }
  }

  ScopedTimer global_timer("All operations finished");

  Buffer buffer;
  {
    ScopedTimer timer("Input file loaded");
    buffer.load(options.input_path);
  }
  {
    ScopedTimer timer("All substitutions performed");

    auto sub_index = 0;
    for (auto const &[pattern, replacement] : substitutions) {
      ++sub_index;

      auto mode = options.should_overwrite ? InsertionMode::Overwrite
                                           : InsertionMode::Insert;
      auto indices = buffer.replace_all(pattern, replacement, mode);

      if (options.wants_verbose_output)
        for (auto index : indices)
          std::cout << "Substitution " << sub_index << " performed at offset 0x"
                    << std::hex << index << std::dec << ".\n";
    }
  }
  {
    ScopedTimer timer("Output file saved");
    buffer.save(options.wants_in_place ? options.input_path
                                       : options.output_path);
  }

  return 0;
}
