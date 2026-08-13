#include "ble_advertiser.h"

#include <NimBLEDevice.h>

#include <string>

#include "cargo_config.h"
#include "role_profiles.h"

size_t buildManufacturerData(Role role, const char* name, uint8_t* out, size_t capacity, const char* tag) {
  const char* useTag = tag ? tag : profileFor(role).tag;
  size_t length = 0;
  while (name[length]) ++length;
  if (capacity < length + 7) return 0;
  out[0] = 0x0d; out[1] = 0xf0;
  out[2] = useTag[0]; out[3] = useTag[1]; out[4] = useTag[2]; out[5] = useTag[3];
  out[6] = profileFor(role).protocolType;
  for (size_t i = 0; i < length; ++i) out[7 + i] = name[i];
  return length + 7;
}

bool BleAdvertiser::begin() {
  NimBLEDevice::init(CARGO_BLE_NAME);
  if (!NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_RANDOM)) { error_ = "random address unavailable"; return false; }
  return true;
}

bool BleAdvertiser::advertise(Role role, const RoleIdentity& identity, const char* tag) {
  stop();
  if (!NimBLEDevice::setOwnAddr(identity.address)) { error_ = "address assignment failed"; return false; }
  NimBLEAdvertisementData data;
  uint8_t packet[20] = {};
  const size_t length = buildManufacturerData(role, identity.name, packet, sizeof(packet), tag);
  if (!length) { error_ = "packet construction failed"; return false; }
  data.setManufacturerData(std::string(reinterpret_cast<char*>(packet), length));
  NimBLEAdvertising* advertising = NimBLEDevice::getAdvertising();
  advertising->setAdvertisementData(data);
  advertising->setConnectableMode(BLE_GAP_CONN_MODE_NON);
  advertising->setDiscoverableMode(BLE_GAP_DISC_MODE_NON);
  if (!advertising->start()) { error_ = "advertising start failed"; return false; }
  identity_ = identity;
  active_ = true;
  error_ = "";
  return true;
}

void BleAdvertiser::stop() { NimBLEDevice::getAdvertising()->stop(); active_ = false; }
bool BleAdvertiser::active() const { return active_; }
const char* BleAdvertiser::lastError() const { return error_; }
RoleIdentity BleAdvertiser::activeIdentity() const { return identity_; }
