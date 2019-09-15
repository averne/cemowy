// Copyright (C) 2019 averne
//
// This file is part of cemowy.
//
// cemowy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// cemowy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with cemowy.  If not, see <http://www.gnu.org/licenses/>.

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
