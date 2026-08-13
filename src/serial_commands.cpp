#include "serial_commands.h"

#include <ctype.h>
#include <string.h>

namespace {
bool equals(const char* left, const char* right) {
  while (*left && *right) {
    if (tolower(static_cast<unsigned char>(*left++)) != tolower(static_cast<unsigned char>(*right++))) return false;
  }
  return *left == 0 && *right == 0;
}

bool twoWords(const char* line, const char* first, const char* second) {
  const size_t firstLength = strlen(first);
  for (size_t i = 0; i < firstLength; ++i) {
    if (!line[i] || tolower(static_cast<unsigned char>(line[i])) != tolower(static_cast<unsigned char>(first[i]))) return false;
  }
  if (line[firstLength] != ' ') return false;
  return equals(line + firstLength + 1, second);
}
}

Command parseCommand(const char* line) {
  if (equals(line, "help")) return {CommandKind::Help, Role::Acolyte};
  if (equals(line, "status")) return {CommandKind::Status, Role::Acolyte};
  if (twoWords(line, "mode", "auto")) return {CommandKind::Auto, Role::Acolyte};
  if (twoWords(line, "mode", "manual")) return {CommandKind::Manual, Role::Acolyte};
  if (equals(line, "next")) return {CommandKind::Next, Role::Acolyte};
  if (twoWords(line, "role", "acolyte")) return {CommandKind::Role, Role::Acolyte};
  if (twoWords(line, "role", "glyph")) return {CommandKind::Role, Role::Glyph};
  if (twoWords(line, "role", "elder")) return {CommandKind::Role, Role::Elder};
  if (twoWords(line, "role", "cthulhu")) return {CommandKind::Role, Role::Cthulhu};
  return {CommandKind::Invalid, Role::Acolyte};
}
