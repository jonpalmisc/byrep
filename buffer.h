//===-- buffer.h - Arbitrary binary data buffer ---------------------------===//
//
// Copyright (c) 2023 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; a full copy
// of the license can be found in the LICENSE.txt file.
//
//===----------------------------------------------------------------------===//

#pragma once

#include <optional>
#include <string>
#include <vector>

/// Defines how an insertion (or replacement) should be performed.
enum class InsertionMode {
  /// Insert data without modifying any existing/subsequent data.
  Insert,

  /// Insert data by overwriting existing data starting at the insertion point.
  Overwrite,
};

using Bytes = std::vector<std::uint8_t>;

/// Arbitrary binary data buffer.
class Buffer {
  std::string m_path;
  Bytes m_data;

public:
  /// Create a new buffer from the file at \p path.
  Buffer(std::string path);

  /// Get the bytes contained by the buffer.
  Bytes const &content() const { return m_data; };

  /// Get a pointer to the bytes contained by the buffer.
  std::uint8_t const *raw_content() const { return m_data.data(); };

  /// Get the size of the buffer in bytes.
  Bytes::size_type size() const { return m_data.size(); };

  /// Replace the next occurance of \p pattern with \p replacement.
  ///
  /// Replacements will be performed as defined by \p mode, with all matches
  /// prior to offset \p start being ignored.
  ///
  /// \returns Index the replacement was performed at, if any
  std::optional<Bytes::size_type>
  replace_next(Bytes const &pattern, Bytes const &replacement,
               InsertionMode mode = InsertionMode::Insert,
               std::size_t start = 0);

  /// Replace all instances of \p pattern with \p replacement.
  ///
  /// Replacements will be performed as defined by \p mode.
  ///
  /// \returns All indices replacements were performed at
  std::vector<Bytes::size_type>
  replace_all(Bytes const &pattern, Bytes const &replacement,
              InsertionMode mode = InsertionMode::Insert);

  /// Save the buffer's contents to the file at \p path.
  void save(std::string const &path) const;
};
