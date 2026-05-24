# StackChan Display

[![PlatformIO CI](https://github.com/botamochi6277/stackchan-display/actions/workflows/platformio-ci.yml/badge.svg)](https://github.com/botamochi6277/stackchan-display/actions/workflows/platformio-ci.yml)

> [!WARNING]
> This library is WIP (*work in progress*) and unstable. Breaking changes can be installed easily!

*StackChan display* is an Arduino library to display stackchan faces.
*StackChan display* depends on only [U5Unified](https://github.com/m5stack/M5Unified) and drawing with it.

This library is based on [m5stack/StackChan](https://github.com/m5stack/StackChan), [botamochi6277/m5stack-avatar](https://github.com/botamochi6277/m5stack-avatar), and  [stack-chan/m5stack-avatar](https://github.com/stack-chan/m5stack-avatar).

## This Library Role for StackChan assembly

*StackChan display* is one of StackChan components to control a display. Even if you use this library alone, “StackChan” will not be complete.

```mermaid
---
config:
  theme: 'redux-dark'
  title: 'M5StackChan architecture'
---
graph LR


planning((planning))

%% sensor inputs

camera[fas:fa-camera camera]-->|cam image|planning
touch[head touch sensor]-->planning
head_rot[head rotation sensor]-->|pan tilt angles|planning
mic[microphone]-->planning
bat[battery sensor]-->planning

%% comm inputs

esp_now[esp now]-.->planning
ble[bluetooth]-.->planning
usb[usb]-.->planning
wifi[WiFi]-.->planning

%% extra

llm[large language model]-.->wifi

%% outputs

planning-->display

subgraph display[display control]
    face([face])
    speech([speech bubble])
    decorators([decorators])
end

display-->display_lib["display lib<br/>(M5Unified)"]

planning-->mot[motion control]
mot-->servo_lib[servo lib]

%% head neopixels on M5StackChan Head
head_led[head LED control]
planning-->head_led[head LED control]
head_led-->led_lib[LED lib]

planning-->speaker
```

## How to use

Install this repository as an Arduino library.

> [!NOTE] TODO: write install command here

---
---

## Developers Note

M5StackChan Face is [m5stack/StackChan](https://github.com/m5stack/StackChan/tree/main/firmware/main/stackchan/avatar) in default.

## TODO for Developers

- [x] Add ellipse eyes
- [x] Add cluster face
- [x] Add Base classes for facial components
- [x] Add build CI tests
- [ ] Increase #decorators
- [ ] Add documents with doxygen
- [x] Add the diagram of system architecture
- [ ] Add pictures of StackChans in the real world
