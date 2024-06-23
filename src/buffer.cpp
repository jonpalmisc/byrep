//
//  Copyright (c) 2023-2024 Jon Palmisciano. All rights reserved.
//
//  Use of this source code is governed by the BSD 3-Clause license; a full copy
//  of the license can be found in the LICENSE.txt file.
//

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

std::optional<size_t> Buffer::replace_next(Bytes const &pat, Bytes const &rep,
                                           SubMode mode, size_t start) {
  // Cannot possibly perform a replacement if the pattern cannot fit in the
  // remaining search region of the buffer.
  if (start + pat.size() >= m_data.size())
    return std::nullopt;

  auto searcher = std::boyer_moore_horspool_searcher(pat.begin(), pat.end());

  auto cursor = m_data.begin() + start, end = m_data.end();
  if ((cursor = std::search(cursor, end, searcher)) != end) {
    auto erase_size = mode == SubMode::Insert ? pat.size() : rep.size();

    m_data.erase(cursor, cursor + erase_size);
    m_data.insert(cursor, rep.begin(), rep.end());
    return cursor - m_data.begin();
  }

  return std::nullopt;
}

std::vector<size_t> Buffer::replace_all(Bytes const &pat, Bytes const &rep,
                                        SubMode mode) {
  std::vector<size_t> rep_indices;

  size_t start = 0;
  while (true) {
    auto index = replace_next(pat, rep, mode, start);
    if (!index)
      break;

    rep_indices.emplace_back(*index);
    start += rep.size();
  }

  return rep_indices;
}
