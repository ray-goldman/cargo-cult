#include "phase_controller.h"

namespace {
Phase nextPhase(Phase phase) {
  switch (phase) {
    case Phase::Acolyte: return Phase::Glyph;
    case Phase::Glyph: return Phase::Elder;
    case Phase::Elder: return Phase::Cthulhu;
    case Phase::Cthulhu: return Phase::Seance;
    default: return Phase::Acolyte;
  }
}
bool hasSlots(Phase phase) { return phase == Phase::Acolyte || phase == Phase::Seance; }
Role phaseRole(Phase phase) {
  switch (phase) {
    case Phase::Glyph: return Role::Glyph;
    case Phase::Elder: return Role::Elder;
    case Phase::Cthulhu: return Role::Cthulhu;
    default: return Role::Acolyte;
  }
}
}

PhaseController::PhaseController(uint32_t phaseMs) : phaseMs_(phaseMs) {}

bool PhaseController::tick(uint32_t now) {
  if (!automatic_) return false;
  const uint32_t elapsed = now - startedAt_;
  if (elapsed >= phaseMs_) {
    phase_ = nextPhase(phase_);
    slot_ = 0;
    startedAt_ = now;
    return true;
  }
  if (hasSlots(phase_)) {
    const uint8_t desired = elapsed / (phaseMs_ / 3);
    if (desired != slot_) {
      slot_ = desired;
      return true;
    }
  }
  return false;
}

void PhaseController::next(uint32_t now) { phase_ = nextPhase(phase_); slot_ = 0; startedAt_ = now; }
void PhaseController::setRole(Role role, uint32_t now) { phase_ = role == Role::Acolyte ? Phase::Acolyte : role == Role::Glyph ? Phase::Glyph : role == Role::Elder ? Phase::Elder : Phase::Cthulhu; slot_ = 0; startedAt_ = now; automatic_ = false; }
void PhaseController::setSeance(uint32_t now) { phase_ = Phase::Seance; slot_ = 0; startedAt_ = now; automatic_ = false; }
void PhaseController::setAutomatic(uint32_t now) { automatic_ = true; slot_ = 0; startedAt_ = now; }
void PhaseController::setManual() { automatic_ = false; }
Phase PhaseController::phase() const { return phase_; }
Role PhaseController::role() const { return phaseRole(phase_); }
bool PhaseController::seance() const { return phase_ == Phase::Seance; }
bool PhaseController::automatic() const { return automatic_; }
uint8_t PhaseController::slot() const { return slot_; }
uint32_t PhaseController::remainingMs(uint32_t now) const { return automatic_ ? (now - startedAt_ >= phaseMs_ ? 0 : phaseMs_ - (now - startedAt_)) : 0; }
