//===-- timer.cpp - Scoped performance timer ------------------------------===//
//
// Copyright (c) 2023 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; a full copy
// of the license can be found in the LICENSE.txt file.
//
//===----------------------------------------------------------------------===//

#include "timer.h"

#include <iostream>

namespace chrono = std::chrono;

static bool g_logging_enabled = false;

void ScopedTimer::set_logging_enabled(bool enabled) {
  g_logging_enabled = enabled;
}

ScopedTimer::ScopedTimer(std::string message)
    : m_message(std::move(message)),
      m_start(chrono::high_resolution_clock::now()) {}

ScopedTimer::~ScopedTimer() {
  if (!g_logging_enabled)
    return;

  auto end = chrono::high_resolution_clock::now();
  auto ms = chrono::duration_cast<chrono::milliseconds>(end - m_start).count();
  std::cout << m_message << " in " << ms << " ms.\n";
}
