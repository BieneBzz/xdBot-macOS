# Building the macOS port

The release package targets **macOS arm64**, **Geometry Dash 2.2081**, and **Geode 5.10.1**.

## Tested dependencies

- Xcode command line tools, CMake, Ninja, and C++23 support.
- Geode SDK 5.10.1, set as `GEODE_SDK`.
- Geode bindings at commit `7f6c2a75742856de88dad354e576dcff8a28e881`.
- Geode CLI 3.9.0.

The SDK and bindings must agree with the game version. A successful link alone does not prove that a mismatched binding set will load in Geometry Dash.

## Build

Set `GEODE_SDK`, `GEODE_BINDINGS_REPO_PATH`, and `GEODE_CLI` to your local installations, then run:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DGEODE_BINDINGS_REPO_PATH="$GEODE_BINDINGS_REPO_PATH" \
  -DGEODE_CLI="$GEODE_CLI" \
  -DGEODE_DONT_INSTALL_MODS=ON
cmake --build build --parallel
```

The package is `build/zilko.xdbot.geode`. In the tested build, `GEODE_SDK` pointed to the Geode 5.10.1 SDK and the bindings checkout above contained bindings for GD 2.2081.

## Verification

The `v2.4.1-prerelease.6` Release build packaged successfully, loaded in the game, and replayed inputs from an `Eon.gdr` From Zero macro. During the in-game check, the click counter advanced and the level reached 3.78% without manual gameplay input after resuming. The entire level and the renderer were not tested.
