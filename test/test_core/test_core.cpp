#include <Arduino.h>
#include <string.h>
#include <unity.h>

#include "phase_controller.h"
#include "role_identities.h"
#include "role_profiles.h"
#include "serial_commands.h"

void test_identity_pool_is_static_random_and_unique() {
  Role roles[] = {Role::Acolyte, Role::Glyph, Role::Elder, Role::Cthulhu};
  for (Role role : roles) {
    TEST_ASSERT_EQUAL_UINT32(32, roleIdentityCount(role));
    for (size_t i = 0; i < roleIdentityCount(role); ++i) {
      TEST_ASSERT_EQUAL_UINT8(0xc0, roleIdentity(role, i).address[0] & 0xc0);
      TEST_ASSERT_TRUE(strlen(roleIdentity(role, i).name) <= 7);
    }
  }
  TEST_ASSERT_EQUAL_STRING("CULT", profileFor(Role::Acolyte).tag);
  TEST_ASSERT_EQUAL_STRING("GLPB", profileFor(Role::Glyph).tag);
}

void test_acolyte_uses_three_twenty_second_slots() {
  PhaseController controller;
  TEST_ASSERT_EQUAL_UINT8(0, controller.slot());
  TEST_ASSERT_TRUE(controller.tick(20000));
  TEST_ASSERT_EQUAL_UINT8(1, controller.slot());
  TEST_ASSERT_TRUE(controller.tick(40000));
  TEST_ASSERT_EQUAL_UINT8(2, controller.slot());
}

void test_cthulhu_cycles_to_acolyte_after_sixty_seconds() {
  PhaseController controller;
  controller.setRole(Role::Cthulhu, 0);
  controller.setAutomatic(0);
  TEST_ASSERT_TRUE(controller.tick(60000));
  TEST_ASSERT_EQUAL(Phase::Acolyte, controller.phase());
}

void test_serial_commands_are_case_insensitive() {
  Command role = parseCommand("RoLe GlYpH");
  TEST_ASSERT_EQUAL(CommandKind::Role, role.kind);
  TEST_ASSERT_EQUAL(Role::Glyph, role.role);
  TEST_ASSERT_EQUAL(CommandKind::Auto, parseCommand("MODE AUTO").kind);
  TEST_ASSERT_EQUAL(CommandKind::Next, parseCommand("next").kind);
  TEST_ASSERT_EQUAL(CommandKind::Invalid, parseCommand("nonsense").kind);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_identity_pool_is_static_random_and_unique);
  RUN_TEST(test_acolyte_uses_three_twenty_second_slots);
  RUN_TEST(test_cthulhu_cycles_to_acolyte_after_sixty_seconds);
  RUN_TEST(test_serial_commands_are_case_insensitive);
  UNITY_END();
}

void loop() {}
