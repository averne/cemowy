#pragma once

#include <cstdint>

#include "platform.h"
#include "utils.hpp"

namespace cmw::err {

constexpr uint32_t Module = 420;

inline constexpr Result FailedFopen        {Module, 0};
inline constexpr Result AlreadyActive      {Module, 1};
inline constexpr Result AlreadyInactive    {Module, 2};

} // namespace cmw::err
