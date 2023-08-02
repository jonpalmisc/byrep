//===-- encoding.h - Data encoding & decoding -----------------------------===//
//
// Copyright (c) 2023 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; a full copy
// of the license can be found in the LICENSE.txt file.
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>

using Bytes = std::vector<std::uint8_t>;

/// Hexadecimal encoding utilities.
class Hex {
public:
  /// Decode a \p hex_string to raw bytes.
  ///
  /// \returns The decoded bytes if successful, or an empty vector on failure
  static Bytes decode(std::string const &input);
};
