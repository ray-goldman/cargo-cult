#pragma once

#include "role_types.h"

enum class CommandKind : uint8_t { Help, Status, Auto, Manual, Role, Next, Invalid };

struct Command {
  CommandKind kind;
  Role role;
};

Command parseCommand(const char* line);
