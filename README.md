# Cargo Cult

Cargo Cult is a serial-first BLE role emulator for classic ESP32 boards. It uses
one legacy advertisement at a time and rotates static-random peer identities.

## Build

Install [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/index.html), then run one of:

```powershell
py -m platformio run -e esp32dev
py -m platformio run -e defmon-wroom
```

For a generic board, explicitly choose its upload port:

```powershell
py -m platformio run -e esp32dev -t upload --upload-port COM7
```

The `defmon-wroom` profile targets the DEFMON ESP-WROOM on `COM5`. Flashing
replaces its current firmware, so make a backup before uploading.

## DEFMON wiring

- SSD1306 OLED: SDA **GPIO 21**, SCL **GPIO 22**, address `0x3C`.
- Next-phase button: **GPIO 26**, active low (internal pull-up enabled).
- Status LED: **GPIO 2**.

The generic `esp32dev` target needs no display, LED, or button. Hardware options
can be overridden with PlatformIO `build_flags` using the `CARGO_*` names in
`include/cargo_config.h`.

## Serial control

Connect at **115200** baud and send a line ending after one of:

```text
help
status
mode auto
mode manual
role acolyte
role glyph
role elder
role cthulhu
seance
next
```

Automatic mode advances Acolyte, Glyph, Elder, Cthulhu, and Seance every 60
seconds. `next` also works from the DEFMON GPIO 26 button.

## Compatibility

Classic ESP32 hardware has one legacy advertising set. Therefore Acolyte and
classic ESP32 Seance peers are **sequential rather than simultaneous**: each
phase uses three 20-second peer slots. A board with multiple advertising sets is
needed for simultaneous peer emulation.
