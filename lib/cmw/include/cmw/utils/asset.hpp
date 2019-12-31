#pragma once

#include <string>

namespace cmw {

// Forward declarations
template <typename T> T read_asset(const std::string &path);
FILE *open_asset(const std::string &path, const std::string &mode = "r");

} // namespace cmw
