# StackChan Display

[![PlatformIO CI](https://github.com/botamochi6277/stackchan-display/actions/workflows/platformio-ci.yml/badge.svg)](https://github.com/botamochi6277/stackchan-display/actions/workflows/platformio-ci.yml)

> [!WARNING]
> This library is WIP (*work in progress*) and unstable. Breaking changes can be installed easily!

<video src="https://github-production-user-asset-6210df.s3.amazonaws.com/14128408/598185770-549d8f23-19b5-4a1a-aad7-f31dbd15b2f8.mp4?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAVCODYLSA53PQK4ZA%2F20260526%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20260526T125213Z&X-Amz-Expires=300&X-Amz-Signature=0ec3d94f839498ef60fea996151d3d9024a500fd0045fb1a1e28f94d940dfc4b&X-Amz-SignedHeaders=host&response-content-type=video%2Fmp4" controls loop></video>

*StackChan display* is an Arduino library to display stackchan faces.
*StackChan display* depends on only [U5Unified](https://github.com/m5stack/M5Unified) and drawing with it.

This library is based on [stack-chan/m5stack-avatar](https://github.com/stack-chan/m5stack-avatar), [botamochi6277/m5stack-avatar](https://github.com/botamochi6277/m5stack-avatar), and [m5stack/StackChan](https://github.com/m5stack/StackChan).

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

> [!NOTE]
> TODO: write install command here

[Demo.ino](./examples/Demo/Demo.ino) is a demo arduino file to draw stackchan faces, please build this.

---
---

## Developers Note

### Differences from m5stack-avatar

These changes are to improve functionality while keeping the code concise.

- No [xTask](https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/system/freertos.html#task-api): If you want to draw face in xtask, please register yourself.
- No `DrawingContext`: It is too complex. [`FacialDrawable.draw()`](./src/faces/FacialDrawable.h) in this library uses [`ExpressionWeight`](./src/Expression.h), [`ColorPalette`](./src/ColorPalette.h), and a few internal parameters. `ExpressionWeight` concept is based on [ShapeKey](https://docs.blender.org/manual/en/dev/animation/shape_keys/index.html).
- No `Avatar`: "Avatar" has to control whole behaviors of stackchan including motor motions and audio behaviors. `Display` class in this library only controls display behaviors.

### TODO for Developers

- [x] Add ellipse eyes
- [x] Add cluster face
- [x] Add Base classes for facial components
- [x] Add build CI tests
- [ ] Increase #decorators
- [x] Add documents with doxygen
- [x] Add the diagram of system architecture
- [ ] Add pictures of StackChans in the real world
- [ ] Migrate more faces from [botamochi6277/m5stack-avatar](https://github.com/botamochi6277/m5stack-avatar)
