# Cargo Cult

Cargo Cult is a serial-first BLE role emulator for classic ESP32 boards. It uses
one legacy advertisement at a time and rotates static-random peer identities.

## Build

Install [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/index.html), then run one of:

```powershell
py -m platformio run -e esp32dev
py -m platformio run -e defmon-wroom
```

Upload to the serial port assigned to your board:

```powershell
py -m platformio run -e esp32dev -t upload --upload-port <serial-port>
```

For the optional OLED/button hardware profile, use `defmon-wroom` in place of
`esp32dev`. Flashing replaces the board's current firmware, so make a backup
before uploading.

## Optional OLED/button wiring

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

Automatic mode advances every phase at 60-second intervals. The normal-role
phases are Acolyte, Glyph, Elder, and Cthulhu; the Seance phase is diagnostic
only on classic ESP32/WROOM hardware. `next` also works from the optional GPIO
26 button.

## Compatibility

| Platform | Normal roles | Seance |
| --- | --- | --- |
| Classic ESP32 / ESP-WROOM | Supported: Acolyte, Glyph, Elder, and Cthulhu rotate with unique static-random identities. | **Not supported.** A WROOM has one legacy advertising set and cannot present the required peers simultaneously. |
| ESP32 with multiple concurrent advertising sets | Depends on the board-specific implementation. | Requires a multi-advertisement implementation; this WROOM firmware does not provide it. |

The `seance` serial command is included for diagnostics, but it does not create
a functional Seance on classic ESP32/WROOM hardware.
