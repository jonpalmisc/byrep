//===-- timer.h - Scoped performance timer --------------------------------===//
//
// Copyright (c) 2023 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; a full copy
// of the license can be found in the LICENSE.txt file.
//
//===----------------------------------------------------------------------===//

#pragma once

#include <chrono>
#include <string>

/// Scoped performance timer.
///
/// Measures execution time and optionally logs the total duration from the
/// moment the timer is constructed to when it is destructed.
class ScopedTimer {
  std::string m_message;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

public:
  ScopedTimer(std::string message);
  ~ScopedTimer();

  /// Enable or disable log messages when the timer goes out of scope.
  static void set_logging_enabled(bool enabled);
};
