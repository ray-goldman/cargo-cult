#include <Arduino.h>
#include <Wire.h>

#include "cargo_config.h"

#if CARGO_HAS_OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

#include "ble_advertiser.h"
#include "phase_controller.h"
#include "role_identities.h"
#include "role_profiles.h"
#include "serial_commands.h"

namespace {
BleAdvertiser ble;
PhaseController controller;
IdentityRotation rotation;
char line[96] = {};
size_t used = 0;
bool lastButton = true;
uint32_t lastButtonPress = 0;
#if CARGO_HAS_OLED
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
bool oledOk = false;
#endif

Role activeRole() {
  if (!controller.seance()) return controller.role();
  return controller.slot() == 0 ? Role::Glyph : controller.slot() == 1 ? Role::Elder : Role::Cthulhu;
}

void draw() {
#if CARGO_HAS_OLED
  if (!oledOk) return;
  oled.clearDisplay();
  oled.setTextSize(1); oled.setTextColor(SSD1306_WHITE); oled.setCursor(0, 0);
  oled.print("Cargo Cult"); oled.setCursor(0, 18);
  oled.print(profileFor(activeRole()).displayName); oled.print(" "); oled.print(ble.activeIdentity().name);
  oled.setCursor(0, 34); oled.print(controller.automatic() ? "AUTO " : "MANUAL ");
  oled.print(controller.remainingMs(millis()) / 1000); oled.print(" sec");
  oled.setCursor(0, 50); oled.print(ble.active() ? "BLE: ADV" : "BLE: ERR"); oled.display();
#endif
}

void startAdvertisement() {
  const Role role = activeRole();
  const RoleIdentity identity = rotation.current(role);
  const char* tag = controller.seance() ? "SEAN" : nullptr;
  if (ble.advertise(role, identity, tag)) {
    rotation.advance(role);
    if (CARGO_STATUS_LED >= 0) digitalWrite(CARGO_STATUS_LED, HIGH);
    Serial.print("role="); Serial.print(profileFor(role).displayName); Serial.print(" peer="); Serial.print(identity.name);
    Serial.print(" mode="); Serial.print(controller.automatic() ? "AUTO" : "MANUAL"); Serial.print(" remaining="); Serial.println(controller.remainingMs(millis()));
  } else {
    if (CARGO_STATUS_LED >= 0) digitalWrite(CARGO_STATUS_LED, LOW);
    Serial.print("ble=ERROR "); Serial.println(ble.lastError());
  }
  draw();
}

void status() {
  Serial.print("role="); Serial.print(profileFor(activeRole()).displayName); Serial.print(" peer="); Serial.print(ble.activeIdentity().name);
  Serial.print(" mode="); Serial.print(controller.automatic() ? "AUTO" : "MANUAL"); Serial.print(" remaining="); Serial.println(controller.remainingMs(millis()));
}

void apply(Command command) {
  const uint32_t now = millis();
  switch (command.kind) {
    case CommandKind::Help: Serial.println("help status mode auto|manual role acolyte|glyph|elder|cthulhu seance next"); return;
    case CommandKind::Status: status(); return;
    case CommandKind::Auto: controller.setAutomatic(now); break;
    case CommandKind::Manual: controller.setManual(); status(); return;
    case CommandKind::Role: controller.setRole(command.role, now); break;
    case CommandKind::Seance: controller.setSeance(now); break;
    case CommandKind::Next: controller.next(now); break;
    default: Serial.println("invalid command"); return;
  }
  startAdvertisement();
}
}

void setup() {
  Serial.begin(115200);
  if (CARGO_STATUS_LED >= 0) { pinMode(CARGO_STATUS_LED, OUTPUT); digitalWrite(CARGO_STATUS_LED, LOW); }
#if CARGO_HAS_NEXT_BUTTON
  pinMode(CARGO_NEXT_BUTTON, INPUT_PULLUP);
#endif
#if CARGO_HAS_OLED
  Wire.begin(CARGO_OLED_SDA, CARGO_OLED_SCL);
  oledOk = oled.begin(SSD1306_SWITCHCAPVCC, 0x3c);
#endif
  if (!ble.begin()) {
    Serial.print("ble=ERROR "); Serial.println(ble.lastError());
    return;
  }
  Serial.println("Cargo Cult ready");
  startAdvertisement();
}

void loop() {
  if (controller.tick(millis())) startAdvertisement();
#if CARGO_HAS_NEXT_BUTTON
  const bool button = digitalRead(CARGO_NEXT_BUTTON);
  if (lastButton && !button && millis() - lastButtonPress >= 150) {
    lastButtonPress = millis();
    apply({CommandKind::Next, Role::Acolyte});
  }
  lastButton = button;
#endif
  while (Serial.available()) {
    const char c = static_cast<char>(Serial.read());
    if (c == '\r' || c == '\n') { if (used) { line[used] = 0; apply(parseCommand(line)); used = 0; } }
    else if (used < sizeof(line) - 1) line[used++] = c;
  }
}
