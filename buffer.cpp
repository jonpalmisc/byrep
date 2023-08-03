//===-- buffer.cpp - Arbitrary binary data buffer -------------------------===//
//
// Copyright (c) 2023 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; a full copy
// of the license can be found in the LICENSE.txt file.
//
//===----------------------------------------------------------------------===//

#include "buffer.h"

#include <fstream>

void Buffer::load(std::string const &path) {
  std::ifstream file(path, std::ios::binary);
  m_data = Bytes{std::istreambuf_iterator<char>(file),
                 std::istreambuf_iterator<char>()};
}

void Buffer::save(std::string const &path) const {
  std::ofstream file(path, std::ios::binary);
  std::copy(m_data.begin(), m_data.end(), std::ostreambuf_iterator<char>(file));
}

std::optional<Bytes::size_type> Buffer::replace_next(Bytes const &pattern,
                                                     Bytes const &replacement,
                                                     InsertionMode mode,
                                                     Bytes::size_type start) {
  // Cannot possibly perform a replacement if the pattern cannot fit in the
  // remaining search region of the buffer.
  if (start + pattern.size() >= m_data.size())
    return std::nullopt;

  auto searcher =
      std::boyer_moore_horspool_searcher(pattern.begin(), pattern.end());

  auto cursor = m_data.begin() + start, end = m_data.end();
  if ((cursor = std::search(cursor, end, searcher)) != end) {
    auto erase_size =
        mode == InsertionMode::Insert ? pattern.size() : replacement.size();

    m_data.erase(cursor, cursor + erase_size);
    m_data.insert(cursor, replacement.begin(), replacement.end());
    return cursor - m_data.begin();
  }

  return std::nullopt;
}

std::vector<Bytes::size_type> Buffer::replace_all(Bytes const &pattern,
                                                  Bytes const &replacement,
                                                  InsertionMode mode) {
  std::vector<Bytes::size_type> replacement_indices;

  Bytes::size_type start = 0;
  while (true) {
    auto index = replace_next(pattern, replacement, mode, start);
    if (!index)
      break;

    replacement_indices.emplace_back(*index);
    start += replacement.size();
  }

  return replacement_indices;
}
