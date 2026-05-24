# StackChan Display

> [!WARNING]
> This library is WIP (*work in progress*) and unstable. Breaking changes can be installed easily!

*StackChan display* is an Arduino library to display stackchan faces.
*StackChan display* depends on only [U5Unified](https://github.com/m5stack/M5Unified) and drawing with it.

This library is based on [m5stack/StackChan](https://github.com/m5stack/StackChan), [botamochi6277/m5stack-avatar](https://github.com/botamochi6277/m5stack-avatar), and  [stack-chan/m5stack-avatar](https://github.com/stack-chan/m5stack-avatar).

## This Library Role for Stack-chan assembly

*StackChan display* is one of stack-chan components to control a display. Even if you use this library alone, “stack-chan” will not be complete.

```mermaid
---
config:
  theme: 'redux-dark'
  title: 'M5StackChan architecture'
---
graph LR

%% sensor inputs

camera[fas:fa-camera camera]-->planning
touch[head touch sensor]-->planning
head_rot[head rotation sensor]-->planning
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
head_led[head led control]
planning-->head_led[head led control]
head_led-->led_lib[led lib]

planning-->speaker
```

## Note

M5StackChan Face is [m5stack/StackChan](https://github.com/m5stack/StackChan/tree/main/firmware/main/stackchan/avatar)

## TODO for Developers

- [x] Add ellipse eyes
- [x] Add cluster face
- [x] Add Base classes for facial components
- [ ] Add build CI tests
- [ ] Increase #decorators
- [ ] Add documents with doxygen
- [x] Add the diagram of system architecture
- [ ] Add pictures of stackchans in the real world
