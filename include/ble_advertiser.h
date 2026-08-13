#pragma once

#include <stddef.h>
#include <stdint.h>

#include "role_identities.h"
#include "role_types.h"

size_t buildManufacturerData(Role role, const char* name, uint8_t* out, size_t capacity, const char* tag = nullptr);

class BleAdvertiser {
 public:
  bool begin();
  bool advertise(Role role, const RoleIdentity& identity, const char* tag = nullptr);
  void stop();
  bool active() const;
  const char* lastError() const;
  RoleIdentity activeIdentity() const;

 private:
  bool active_ = false;
  RoleIdentity identity_ = {nullptr, ""};
  const char* error_ = "not started";
};
