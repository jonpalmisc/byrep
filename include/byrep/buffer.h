//
//  Copyright (c) 2023-2024 Jon Palmisciano. All rights reserved.
//
//  Use of this source code is governed by the BSD 3-Clause license; a full copy
//  of the license can be found in the LICENSE.txt file.
//

#pragma once

#include <optional>
#include <string>
#include <vector>

namespace byrep {

/// Defines how a substitution should be performed.
enum class SubMode {
  /// Insert data without modifying any existing/subsequent data.
  Insert,

  /// Insert data by overwriting existing data starting at the insertion point.
  Overwrite,
};

using Bytes = std::vector<uint8_t>;

/// Arbitrary binary data buffer.
class Buffer {
  Bytes m_data;

public:
  Buffer() = default;

  /// Get the bytes contained by the buffer.
  Bytes const &content() const { return m_data; };

  /// Get a pointer to the bytes contained by the buffer.
  uint8_t const *raw_content() const { return m_data.data(); };

  /// Get the size of the buffer in bytes.
  size_t size() const { return m_data.size(); };

  /// Load the buffer with the contents of the file at \p path.
  void load(std::string const &path);

  /// Save the buffer's contents to the file at \p path.
  void save(std::string const &path) const;

  /// Replace the next occurance of \p pat with \p rep.
  ///
  /// Replacements will be performed as defined by \p mode, with all matches
  /// prior to offset \p start being ignored.
  ///
  /// \returns Index the replacement was performed at, if any
  std::optional<size_t> replace_next(Bytes const &pat, Bytes const &rep,
                                     SubMode mode = SubMode::Insert,
                                     size_t start = 0);

  /// Replace all instances of \p pat with \p rep.
  ///
  /// Replacements will be performed as defined by \p mode.
  ///
  /// \returns All indices replacements were performed at
  std::vector<size_t> replace_all(Bytes const &pat, Bytes const &rep,
                                  SubMode mode = SubMode::Insert);
};

} // namespace byrep
