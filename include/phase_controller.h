#pragma once

#include <stdint.h>

#include "role_types.h"

class PhaseController {
 public:
  explicit PhaseController(uint32_t phaseMs = 60000);
  bool tick(uint32_t now);
  void next(uint32_t now);
  void setRole(Role role, uint32_t now);
  void setSeance(uint32_t now);
  void setAutomatic(uint32_t now);
  void setManual();
  Phase phase() const;
  Role role() const;
  bool seance() const;
  bool automatic() const;
  uint8_t slot() const;
  uint32_t remainingMs(uint32_t now) const;

 private:
  uint32_t phaseMs_;
  uint32_t startedAt_ = 0;
  Phase phase_ = Phase::Acolyte;
  bool automatic_ = true;
  uint8_t slot_ = 0;
};
