#pragma once

#include "role_types.h"

struct RoleProfile {
  Role role;
  const char* displayName;
  const char* tag;
  uint8_t protocolType;
};

const RoleProfile& profileFor(Role role);
