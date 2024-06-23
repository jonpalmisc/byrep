//
//  Copyright (c) 2023-2024 Jon Palmisciano. All rights reserved.
//
//  Use of this source code is governed by the BSD 3-Clause license; a full copy
//  of the license can be found in the LICENSE.txt file.
//

#include "byrep/buffer.h"
#include "byrep/encoding.h"
#include "options.h"

#include <iostream>

using namespace byrep;

/// Find-and-replace substitution.
using Sub = std::pair<Bytes, Bytes>;

/// Substitution syntax parsing error.
enum class SubParseError { Ok, MissingSeparator, BadPattern, BadReplacement };

SubParseError parse_sub(std::string const &literal, Sub &result) {
  auto sep_index = literal.find('/');
  if (sep_index == std::string::npos)
    return SubParseError::MissingSeparator;

  auto pat_hex = literal.substr(0, sep_index);
  result.first = Hex::decode(pat_hex);
  if (result.first.empty())
    return SubParseError::BadPattern;

  auto rep_hex = literal.substr(sep_index + 1, literal.size() - sep_index);
  result.second = Hex::decode(rep_hex);
  if (result.second.empty())
    return SubParseError::BadReplacement;

  return SubParseError::Ok;
}

int main(int argc, char **argv) {
  Options options(argc, argv);

  std::vector<Sub> subs(options.subs.size());
  for (size_t i = 0; i < subs.size(); ++i) {
    switch (parse_sub(options.subs[i], subs[i])) {
    case SubParseError::MissingSeparator:
      std::cerr << "Error: Failed to parse substitution " << i
                << "; missing separator.\n";
      return 1;
    case SubParseError::BadPattern:
      std::cerr << "Error: Failed to parse substitution " << i
                << "; failed to hex-decode pattern.\n";
      return 1;
    case SubParseError::BadReplacement:
      std::cerr << "Error: Failed to parse substitution " << i
                << "; failed to hex-decode replacement.\n";
      return 1;
    default:
      break;
    }
  }

  Buffer buffer;
  buffer.load(options.input_path);

  auto sub_index = 0;
  for (auto const &[pat, rep] : subs) {
    ++sub_index;

    auto mode = options.should_overwrite ? SubMode::Overwrite : SubMode::Insert;
    auto indices = buffer.replace_all(pat, rep, mode);

    for (auto index : indices)
      std::cout << "Substitution " << sub_index << " performed at offset 0x"
                << std::hex << index << std::dec << ".\n";
  }

  buffer.save(options.wants_in_place ? options.input_path
                                     : options.output_path);

  return 0;
}
