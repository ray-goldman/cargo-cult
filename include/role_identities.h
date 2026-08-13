#pragma once

#include <stddef.h>
#include <stdint.h>

#include "role_types.h"

constexpr size_t kRoleIdentityAddressLength = 6;
constexpr size_t kIdentitiesPerRole = 32;

struct RoleIdentity {
  const uint8_t* address;
  const char* name;
};

size_t roleIdentityCount(Role role);
RoleIdentity roleIdentity(Role role, size_t index);

class IdentityRotation {
 public:
  RoleIdentity current(Role role) const;
  size_t index(Role role) const;
  void advance(Role role);
  void reset();

 private:
  uint8_t index_[4] = {};
};
