//===-- encoding.cpp - Data encoding & decoding ---------------------------===//
//
// Copyright (c) 2023 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; a full copy
// of the license can be found in the LICENSE.txt file.
//
//===----------------------------------------------------------------------===//

#include "encoding.h"

Bytes Hex::decode(std::string const &hex_string) {
  auto input_size = hex_string.size();
  if (input_size % 2 != 0)
    return {};

  Bytes result;
  result.reserve(input_size / 2);

  try {
    for (size_t i = 0; i < input_size; i += 2)
      result.emplace_back(std::stoul(hex_string.substr(i, 2), 0, 16));
  } catch (...) {
    return {};
  }

  return result;
}
