#include "role_profiles.h"

namespace {
constexpr RoleProfile kProfiles[] = {
    {Role::Acolyte, "Acolyte", "CULT", 1},
    {Role::Glyph, "Glyph", "GLPB", 2},
    {Role::Elder, "Elder", "ELDR", 3},
    {Role::Cthulhu, "Cthulhu", "CTHU", 4},
};
}

const RoleProfile& profileFor(Role role) {
  const uint8_t index = static_cast<uint8_t>(role);
  return kProfiles[index >= 1 && index <= 4 ? index - 1 : 0];
}
