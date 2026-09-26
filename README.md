# xdBot for macOS (unofficial port)

Apple Silicon build of [Zilko's xdBot](https://github.com/ZiLko/xdBot) for **Geometry Dash 2.2081** and **Geode 5.10.1**. This repository preserves the upstream history and contains the macOS port and subsequent fixes. It is not maintained by Zilko.

## Download

Download the latest `zilko.xdbot.geode` from the [macOS Releases page](https://github.com/BieneBzz/xdBot-macOS/releases). The current macOS build is `v2.4.1-prerelease.6`. It is for macOS arm64; the upstream Windows and Android builds are available from the [original xdBot releases](https://github.com/ZiLko/xdBot/releases).

## Install

1. Quit Geometry Dash.
2. Put `zilko.xdbot.geode` in `Geometry Dash.app/Contents/geode/mods/`. If an older `zilko.xdbot.geode` is there, replace it.
3. Launch Geometry Dash through Steam. Open the xdBot menu from the pause menu or with **Control+Shift+M**.

This build expects Geometry Dash 2.2081, Geode 5.10.1, and an Apple Silicon Mac. The `.geode` file is the mod; it is not a standalone app.

## Current fixes

- Practice recordings retain inputs after deaths and checkpoint respawns.
- Playback uses the macOS gameplay update path, so recorded inputs are actually replayed.
- The macro folder resolves under the current macOS user's Documents directory instead of a previous test user's path.

Playback was tested in-game with a From Zero `Eon.gdr` recording through the beginning of the level. A full-level playback and video render have not been verified.

## Source and builds

See [BUILDING-macOS.md](BUILDING-macOS.md) for the tested toolchain and build command. Releases contain the built `.geode` file, so downloading source is not required for installation.

The upstream project was archived by its owner. This repository preserves attribution and its original history. No license file was present in the upstream source, so this repository does not claim a new license for that code or its assets.
