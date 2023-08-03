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

  Buffer buffer(options.input_path);
  for (auto const &[pattern, replacement] : substitutions)
    buffer.replace_all(pattern, replacement,
                       options.should_overwrite ? InsertionMode::Overwrite
                                                : InsertionMode::Insert);

  buffer.save(options.wants_in_place ? options.input_path
                                     : options.output_path);

  return 0;
}
