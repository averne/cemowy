#pragma once

#include <cstdint>

#include "platform.h"
#include "utils.hpp"

namespace cmw::err {

constexpr uint32_t Module = 0xf00;

constexpr Result FailedFopen        {Module, 0};
constexpr Result AlreadyActive      {Module, 1};
constexpr Result AlreadyInactive    {Module, 1};

} // namespace cmw::err
