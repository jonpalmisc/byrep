//
//  Copyright (c) 2023-2024 Jon Palmisciano. All rights reserved.
//
//  Use of this source code is governed by the BSD 3-Clause license; a full copy
//  of the license can be found in the LICENSE.txt file.
//

#pragma once

#include <string>
#include <vector>

/// Program options.
class Options {
  /// Show program usage, then exit.
  ///
  /// Will exit with a non-zero exit code if \p was_requested is false,
  /// indicating this message was shown due to a usage error. Full help and
  /// option descriptions, etc. will only be shown if \p was_requested is true.
  void show_usage_and_exit(bool was_requested);

public:
  /// Indicates if the user has requested help.
  bool wants_help;

  /// Indicates if the input file should be patched in-place (modified) rather
  /// than creating a separate output file.
  bool wants_in_place;

  /// Indicates if replacements should be done in overwrite mode.
  bool should_overwrite;

  /// Input file path.
  std::string input_path;

  /// Output file path; may be empty if \c wants_in_place is set.
  std::string output_path;

  /// List of substitutions (in literal form).
  std::vector<std::string> subs;

  /// Create an options instance by parsing CLI arguments.
  Options(int argc, char **argv);
};
