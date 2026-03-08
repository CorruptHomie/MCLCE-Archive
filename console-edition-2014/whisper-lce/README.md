# Minecraft Console Edition 2014 (whisper-lce)

## Download This Folder

```bash
git clone --depth 1 --filter=blob:none --sparse https://github.com/M2noa/MCLCE-Archive.git
cd MCLCE-Archive
git sparse-checkout set console-edition-2014/whisper-lce
```

[Download via Browser](https://download-directory.github.io/?url=https://github.com/M2noa/MCLCE-Archive/tree/main/console-edition-2014/whisper-lce)

Leaked Minecraft Console Edition source code from 2014.

## Build Instructions

### Requirements
- **Visual Studio 2026** (v145 toolset) or **Visual Studio 2012** (v110 toolset)
- Windows 10/11 x64
- Platform: **x64** only

### Building

1. Open `Minecraft.Client\Minecraft.Client.sln` in Visual Studio
2. Select **Release | x64** configuration
3. Build Solution (Ctrl+Shift+B)

Output: `Minecraft.Client\x64\Release\Minecraft.exe`

### Toolset Notes
- **v145** (VS 2026 / MSVC 14.50) - Current default, may have compatibility issues with precompiled libs
- **v110** (VS 2012) - Original toolset, guaranteed compatibility with bundled libraries

If you encounter linker errors with v145, switch to v110 toolset in project properties.

## Project Structure

- **Minecraft.Client/** - Main game client code
- **Minecraft.World/** - World/level generation and management

## Features Added

- 280 FPS cap (option 0)
- Runtime debug settings window (Windows64 builds)
- Removed Sleep(0) throttling from main loop
- Fixed D3D11 debug layer warnings
- Updated to v145 toolset for VS 2026

## License

This is leaked source code. Use at your own risk.
