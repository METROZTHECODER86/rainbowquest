# RainbowQuest (Beat Saber 1.40.8 port)

Makes notes, sabers, lights and walls fade through the rainbow. Original mod by Unifox.
Ported to Beat Saber Quest **1.40.8** (Scotland2 loader, bs-cordl, BSML).

## Build
Requires qpm, CMake, Ninja and Android NDK 27 (`qpm ndk download 27`).

    qpm restore
    cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -B build
    cmake --build ./build
    qpm qmod manifest
    qpm qmod zip -i ./build/libRainbowQuest.so cover.png

Install the resulting .qmod with ModsBeforeFriday.

## Settings
- Rainbow Enabled
- Rainbow Speed (0.4-0.5 recommended)

Turn it off for Chroma maps.
