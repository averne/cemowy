#pragma once

#include <cstdint>

#include "utils.hpp"
#include "platform.h"

namespace cmw::err {

constexpr uint32_t Module = 420;

#define DECL_ERR(name, desc) inline constexpr Result name = {Module, desc}

DECL_ERR(FailedFopen,         0);
DECL_ERR(AlreadyActive,       1);
DECL_ERR(AlreadyInactive,     2);

#undef DECL_ERR

} // namespace cmw::err
